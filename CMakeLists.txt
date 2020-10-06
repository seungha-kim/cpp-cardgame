cmake_minimum_required(VERSION 3.17)
project(cardgame)
set(CMAKE_CXX_STANDARD 17)


### for iOS
# TODO: bitcode 손상되는 문제
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_NAME_DIR "@rpath")
# TODO: fat binary
#set(CMAKE_IOS_INSTALL_COMBINED FALSE)

# TODO: CMAKE_SYSTEM_NAME 만 가지고는 안되네..
if (${PP_FORCE_EMSCRIPTEN})
    set(PP_EMSCRIPTEN 1)
    add_compile_definitions(PP_EMSCRIPTEN=1)
elseif ((CMAKE_SYSTEM_NAME EQUAL "Darwin") OR (${PP_FORCE_MACOS}))
    set(PP_MACOS 1)
    add_compile_definitions(PP_MACOS=1)
elseif ((CMAKE_SYSTEM_NAME EQUAL "iOS") OR (${PP_FORCE_IOS}))
    set(PP_IOS 1)
    add_compile_definitions(PP_IOS=1)
endif()


### Setup for cmake-conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    # TODO: v0.15.0에 문제가 있어서 임시로 develop으로 박아둠. v0.16.0이 나오면 그걸 쓰면 됨
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/develop/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

if (PP_MACOS)
    # TODO: 경우에 따라 패키지 선택적으로 설치
    conan_cmake_run(
            CONANFILE conanfile-macos.txt
            BASIC_SETUP CMAKE_TARGETS
            BUILD missing)
endif()


### Targets

if (PP_EMSCRIPTEN)
    set(CARD_GAME_RENDERER_TYPE STATIC)
else()
    set(CARD_GAME_RENDERER_TYPE SHARED)
endif()

add_library(CardGameRenderer
        ${CARD_GAME_RENDERER_TYPE}
        CardGameRenderer.cpp
        CardGameRenderer.h)

# TODO: install_name에 rpath 넣으려고 한 것. 꼭 이렇게 할 필요 없음. 어차피 이렇게 해도 bitcode 손상됨.
install(TARGETS CardGameRenderer
        LIBRARY DESTINATION lib)

if (PP_EMSCRIPTEN)
    add_subdirectory(web)

    add_executable(cardgame-web
            web/web_main.cpp)

    target_link_libraries(cardgame-web
            WebGLContext)

    target_link_libraries(cardgame-web
            CardGameRenderer)

    # TODO: GLESv2 링크가 안된거같다 https://stackoverflow.com/questions/50959871/generate-web-assembly-from-cmake-project-with-a-specified-html-output
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
#    set_target_properties(cardgame-web PROPERTIES OUTPUT_NAME "cardgame-web.html")
endif()

if (PP_MACOS)
    add_executable(cardgame
            main.cpp)

    add_library(CardGameInitializer
            SHARED
            CardGameInitializer.cpp
            CardGameInitializer.h)

    target_link_libraries(CardGameRenderer
            PRIVATE CONAN_PKG::angle)

    target_include_directories(CardGameRenderer
            PRIVATE ${CONAN_INCLUDE_DIRS})

    target_link_libraries(CardGameInitializer
            PRIVATE CONAN_PKG::angle)

    target_include_directories(CardGameInitializer
            PRIVATE ${CONAN_INCLUDE_DIRS})

    # TODO: Qt instead of glfw (glfw가 ANGLE 지원하지 않음)
    target_link_libraries(cardgame
            CONAN_PKG::glfw)

    target_include_directories(cardgame
            PRIVATE ${CONAN_INCLUDE_DIRS})
endif()

if (PP_IOS)
    target_link_libraries(CardGameRenderer
            PRIVATE "-framework OpenGLES" "-framework Foundation")
endif()


# TODO: use cmake options instead of using raw flags
# TODO: non-apple
#if (APPLE)
#    target_link_options(cardgame
#            PRIVATE "-Wl,-rpath,@executable_path")
#endif()
