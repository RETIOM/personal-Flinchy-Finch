#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include <SFML/Graphics.hpp>

#include "../include/animation.h"
#include "../include/player.h"
#include "../include/ground.h"
#include "../include/pipe.h"

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
    window.setFramerateLimit(144);

    // TODO:
    // + modify player(make it store a window, move animation into it, make speed relative to window size)
    // + add pipes
    // - add speed calculation function(that would be in game.h scorelogscore)
    // - add menu(main(Play(also space), difficulty, mode, exit), restart(play, main menu, score(last, best))
    // - add score (:))


    sf::Texture groundTexture("./assets/ground.png");
    Ground ground(window, groundTexture);


    sf::RectangleShape bg(sf::Vector2f(display_width,display_height-112.0f));
    const sf::Texture bgTexture("./assets/bg.png");
    bg.setTexture(&bgTexture);
    bg.setPosition(sf::Vector2f(0,0));


    sf::Texture birdTexture("./assets/bird.png");
    Player bird(window, birdTexture, sf::Vector2u(3,1), 0.1f);


    sf::Texture topPipeTexture("./assets/tube1.png");
    sf::Texture bottomPipeTexture("./assets/tube2.png");

    // Pipe pipe(window, topPipeTexture, bottomPipeTexture);

    // **********************
    // Working on Pipe Container
    std::deque<Pipe> pipes;

    pipes.emplace_back(Pipe(window, topPipeTexture, bottomPipeTexture));


    int counter = 0;

    // **********************

    float deltaTime = 0.0f;
    sf::Clock clock;


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();


        bird.update(deltaTime);
        ground.update(deltaTime);

        window.clear();


        if (window.getSize().x - (pipes.back().getXPosition()+pipes.back().getThickness()) >= 200.f )
            pipes.emplace_back(Pipe(window, topPipeTexture, bottomPipeTexture));


        window.draw(bg);
        bird.draw();

        if (pipes.front().isOutOfBounds()) {pipes.pop_front();}
        for (auto &piper : pipes) {
            piper.update(deltaTime);
            if (piper.isOutOfBounds()) {pipes.pop_front(); continue; }

            piper.draw();
        }
        ground.draw();
        window.display();
    }
}
