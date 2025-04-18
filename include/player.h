//
// Created by tommeh on 04.04.25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "animation.h"


class Player {
public:
    Player(sf::RenderWindow& window, sf::Texture& texture, sf::Vector2u imageCount, float switchTime);
    ~Player() = default;
    void update(float deltaTime);
    void draw() const;
private:
    bool hasCollided();

private:
    sf::RenderWindow& _window;
    sf::Angle angle;
    float ySpeed;
    Animation animation;
    sf::Sprite body;
    float gravity = -1500.0f;
    float jumpSpeed = -400.f;
    float rotation = 200.0f;
    int score = 0;

    bool wasSpacePressed = false;

    bool isAlive = true;
};

#endif //PLAYER_H
