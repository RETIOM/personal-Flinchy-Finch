cmake_minimum_required(VERSION 3.28)
project(FlinchyFinch LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(PROJECT_ROOT_DIR $(CMAKE_SOURCE_DIR))

find_package(Eigen3 REQUIRED)

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
        include/pipe_manager.h
        src/pipe_manager.cpp
        include/player_manager.h
        src/player_manager.cpp
        include/difficulty.h
        include/mode.h
        include/game_state.h
        include/synapse.h
        include/agent_manager.h
        src/genome.cpp
        include/node.h
        include/genome.h
        include/node_type.h
        src/node.cpp
        include/AI_player.h
        src/AI_player.cpp
        src/AI_manager.cpp)

target_compile_features(main PRIVATE cxx_std_20)

target_link_libraries(main PRIVATE SFML::Graphics Eigen3::Eigen)