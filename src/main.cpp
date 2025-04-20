#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include <SFML/Graphics.hpp>

#include "../include/animation.h"
#include "../include/player.h"
#include "../include/ground.h"
#include "../include/pipe.h"
#include "../include/pipe_controller.h"

int generate_clamped_normal_int() {
    // Make all of these related to the window size
    int mean = 175;
    int stddev = 50;
    int min = 50;
    int max = 300;
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::normal_distribution<double> dist(mean, stddev);

    double value = dist(rng);
    return std::clamp(static_cast<int>(std::floor(value)), min, max);
}


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
    // - create game class
    // - add speed calculation function(that would be in game.h scorelogscore)
    // - add menu(main(Play(also space), difficulty, mode, exit), restart(play, main menu, score(last, best))


    sf::Texture groundTexture("./assets/ground.png");
    Ground ground(window, groundTexture);


    sf::RectangleShape bg(sf::Vector2f(display_width,display_height-112.0f));
    const sf::Texture bgTexture("./assets/bg.png");
    bg.setTexture(&bgTexture);
    bg.setPosition(sf::Vector2f(0,0));


    sf::Texture topPipeTexture("./assets/tube1.png");
    sf::Texture bottomPipeTexture("./assets/tube2.png");

    int score = 0;
    PipeController pipes(window, topPipeTexture, bottomPipeTexture, score);

    sf::Texture birdTexture("./assets/bird.png");
    Player bird(pipes, window, birdTexture, sf::Vector2u(3,1), 0.1f);




    sf::Font font("./assets/PressStart2P-Regular.ttf");
    sf::Text text(font);

    text.setCharacterSize(window.getSize().y / 10);
    text.setOrigin(sf::Vector2f(text.getLocalBounds().size.x/2, text.getLocalBounds().size.y/2));
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(text.getCharacterSize() / 10);
    text.setPosition(sf::Vector2f(display_width / 2,text.getLocalBounds().size.y/2+display_height / 10));

    float deltaTime = 0.0f;
    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();


        if (bird.Alive()) {
            bird.update(deltaTime);

            ground.update(deltaTime);

            window.clear();
            text.setString(std::to_string(score));

            window.draw(bg);
            bird.draw();
            pipes.update(deltaTime);
            ground.draw();
            window.draw(text);
            window.display();
        }
    }
}
