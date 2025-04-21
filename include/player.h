//
// Created by tommeh on 04.04.25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "animation.h"
#include "pipe_manager.h"


class Player {
public:
    Player(PipeManager& pipes, sf::RenderWindow& window, sf::Texture& texture);
    ~Player() = default;
    void update(float deltaTime);
    void draw() const;
    void reset();
    bool alive() const {return isAlive;};
private:
    void checkCollision();

private:
    sf::RenderWindow& _window;
    sf::Angle angle;
    float ySpeed;
    Animation animation;
    sf::Sprite body;
    PipeManager& _pipes;
    sf::Vector2f size;

    float gravity = -1500.0f;
    float jumpSpeed = -400.f;
    float rotation = 200.0f;

    bool wasSpacePressed = false;

    bool isAlive = true;


};

#endif //PLAYER_H
