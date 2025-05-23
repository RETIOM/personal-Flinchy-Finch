#include <SFML/Graphics.hpp>
#include <iostream>

#include "../include/game.h"
#include "../include/difficulty.h"
#include "../include/game_state.h"
#include "../include/mode.h"

int main()
{
    auto display_width = 480u;
    auto display_height = 640u;

    auto window = sf::RenderWindow(sf::VideoMode({display_width, display_height}), "Flinchy Finch(TM)",
        sf::Style::Default, sf::State::Windowed);

    // TODO:
    // + scale bird with resolution
    // + add collision
    // + add scoring
    // + create player manager
    // + create game class
    // + add speed calculation function(that would be in game.h)
    // + add and handle game states(startup, ready, running, paused)
    // + create abstract class AgentManager and put it above player&AIPlayer
    // + refactor Game object to work with AgentManager(store pointer to object)
    // + create class Agents and its components(neuron, synapse, network)/(Genome, genome translation or smth)
    // - add a bias node(node 0, can be input to all but input nodes)
    // - add network visualization :)
    // - add menu(main(Play(also space), difficulty, mode, exit), restart(play, main menu, score(last, best))
    // - add central CONFIG file for all options(screen size, speeds, GENETIC PARAMS)


    // Set manually
    auto difficulty = Difficulty::EASY;
    auto mode = Mode::AI;

    Game game(difficulty, mode, window);

    GameState state;
    mode == Mode::MANUAL ? state=GameState::READY : state=GameState::RUNNING;

    float deltaTime = 0.0f;
    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                    if (state == GameState::READY && keyPress->scancode == sf::Keyboard::Scan::Space) {
                        game.reset();
                        window.clear();
                        game.draw();
                        window.display();
                        state = GameState::RUNNING;
                    }
                    if (state == GameState::RUNNING && keyPress->scancode == sf::Keyboard::Scan::Escape) {
                        state = GameState::PAUSED;
                    }
                    if (state == GameState::PAUSED && keyPress->scancode == sf::Keyboard::Scan::Space) {
                        state = GameState::RUNNING;
                    }
                }
            }

        if (mode == Mode::MANUAL && game.getIsDone()) {
            state = GameState::READY;
        }


        deltaTime = clock.restart().asSeconds();
        if (state == GameState::RUNNING) {
            window.clear();
            game.update(deltaTime);
            game.draw();
            window.display();
        }
    }
}
