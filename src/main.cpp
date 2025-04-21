#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include <SFML/Graphics.hpp>

#include "../include/animation.h"
#include "../include/player.h"
#include "../include/ground.h"
#include "../include/pipe.h"
#include "../include/pipe_manager.h"
#include "../include/player_manager.h"
#include "../include/game.h"
#include "../include/difficulty.h"
#include "../include/mode.h"



int main()
{
    auto display_width = 800u;
    auto display_height = 600u;

    auto window = sf::RenderWindow(sf::VideoMode({display_width, display_height}), "CMake SFML Project",
        sf::Style::Default, sf::State::Windowed);
    // window.setFramerateLimit(144);

    // TODO:
    // + scale bird with resolution
    // + add collision
    // + add scoring
    // + create player manager
    // + create game class
    // + add speed calculation function(that would be in game.h)
    // - add and handle game states(startup, ready, running, paused)
    // - create class Agents and its components(neuron, synapse, network)
    // - add menu(main(Play(also space), difficulty, mode, exit), restart(play, main menu, score(last, best))



    auto difficulty = Difficulty::HARD;
    auto mode = Mode::MANUAL;

    Game game(difficulty, mode, window);

    float deltaTime = 0.0f;
    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();

        if (!game.getIsDone()) {
            window.clear();
            game.update(deltaTime);
            game.draw();
            window.display();
        }
    }
}
