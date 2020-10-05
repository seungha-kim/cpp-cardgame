cmake_minimum_required(VERSION 3.17)
project(cardgame)
set(CMAKE_CXX_STANDARD 17)


### for iOS
# TODO: bitcode 손상되는 문제
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_NAME_DIR "@rpath")
# TODO: fat binary
#set(CMAKE_IOS_INSTALL_COMBINED FALSE)


if ((CMAKE_SYSTEM_NAME EQUAL "Darwin") OR (${PP_FORCE_MACOS}))
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

add_library(CardGameRenderer
        SHARED
        CardGameRenderer.cpp
        CardGameRenderer.h)

install(TARGETS CardGameRenderer
        DESTINATION _install)

add_executable(cardgame
        main.cpp)

if (PP_MACOS)
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
