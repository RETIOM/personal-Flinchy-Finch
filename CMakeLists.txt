cmake_minimum_required(VERSION 3.28)
project(FlinchyFinch LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(PROJECT_ROOT_DIR $(CMAKE_SOURCE_DIR))

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp
        src/player.cpp
        include/player.h
        src/pipe.cpp
        include/pipe.h
        src/game.cpp
        include/game.h
        src/animation.cpp
        include/animation.h
        src/ground.cpp
        include/ground.h
        include/pipe_controller.h
        src/pipe_controller.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
