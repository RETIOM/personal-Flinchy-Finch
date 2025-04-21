//
// Created by tommeh on 17.04.25.
//

#ifndef GROUND_H
#define GROUND_H

#include <SFML/Graphics.hpp>

class Ground {
public:
    Ground(sf::RenderWindow& window, sf::Texture& texture);
    ~Ground() = default;

    void update(float deltaTime, float velocity=100.f);
    void draw() const;
    void reset();
private:
    sf::RenderWindow& _window;
    sf::Sprite _ground1;
    sf::Sprite _ground2;
};

#endif //GROUND_H
