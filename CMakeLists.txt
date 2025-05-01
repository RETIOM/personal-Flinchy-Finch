cmake_minimum_required(VERSION 3.28)
project(FlinchyFinch LANGUAGES CXX)


# Comment to run tests
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(Eigen3 REQUIRED)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

# Comment to run tests
set(SANITIZER_FLAGS -fsanitize=address -fno-omit-frame-pointer)

add_library(flinchy_core
        src/player.cpp
        src/pipe.cpp
        src/game.cpp
        src/animation.cpp
        src/ground.cpp
        src/pipe_manager.cpp
        src/player_manager.cpp
        src/genome.cpp
        src/node.cpp
        src/AI_player.cpp
        src/AI_manager.cpp
        include/utils.h
        src/utils.cpp
)

target_include_directories(flinchy_core PUBLIC include)
target_link_libraries(flinchy_core PUBLIC SFML::Graphics Eigen3::Eigen)
target_compile_features(flinchy_core PUBLIC cxx_std_20)

# Comment to run tests
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(flinchy_core PRIVATE ${SANITIZER_FLAGS} -g)
    target_link_options(flinchy_core PRIVATE ${SANITIZER_FLAGS})
endif()

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE flinchy_core)

# Comment to run tests
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE ${SANITIZER_FLAGS} -g)
    target_link_options(main PRIVATE ${SANITIZER_FLAGS})
endif()

enable_testing()
add_subdirectory(tests)
