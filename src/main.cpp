#include <iostream>
#include <SFML/Graphics.hpp>

#include "../include/animation.h"
#include "../include/player.h"
#include "../include/ground.h"

int main()
{
    auto display_width = 800u;
    auto display_height = 600u;

    auto window = sf::RenderWindow(sf::VideoMode({display_width, display_height}), "CMake SFML Project",
        sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(60);

    // TODO:
    // - modify player(make it store a window, move animation into it, make speed relative to window size)
    // - add pipes
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
    Player bird(&birdTexture, sf::Vector2u(3,1), 0.15f);



    float deltaTime = 0.0f;
    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();

        std::cout<<deltaTime<<std::endl;

        bird.update(deltaTime);
        ground.update(deltaTime);

        window.clear();

        window.draw(bg);
        bird.draw(window);
        ground.draw();
        // window.draw(ground);
        window.display();
    }
}
