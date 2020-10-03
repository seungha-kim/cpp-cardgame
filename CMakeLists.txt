cmake_minimum_required(VERSION 3.17)
project(cardgame)
set(CMAKE_CXX_STANDARD 17)

### Setup for cmake-conan

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    # TODO: v0.15.0에 문제가 있어서 임시로 develop으로 박아둠. v0.16.0이 나오면 그걸 쓰면 됨
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/develop/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(
        CONANFILE conanfile.txt
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

### Targets

add_library(CardGameRenderer
        SHARED
        CardGameRenderer.cpp
        CardGameRenderer.h)

target_link_libraries(CardGameRenderer
        PRIVATE CONAN_PKG::angle)

target_include_directories(CardGameRenderer
        PRIVATE ${CONAN_INCLUDE_DIRS})

add_executable(cardgame
        main.cpp)

target_link_libraries(cardgame
        PRIVATE CONAN_PKG::angle CONAN_PKG::glfw)

target_include_directories(cardgame
        PRIVATE ${CONAN_INCLUDE_DIRS})

# TODO: use cmake options instead of using raw flags
# TODO: non-apple
if (APPLE)
    target_link_options(cardgame
            PRIVATE "-Wl,-rpath,@executable_path")
endif()
