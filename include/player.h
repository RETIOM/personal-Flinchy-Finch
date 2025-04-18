//
// Created by tommeh on 04.04.25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "animation.h"


class Player {
public:
    Player(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
    ~Player() = default;
    void update(float deltaTime);
    void draw(sf::RenderWindow &window) const;

private:
    sf::Angle angle;
    float ySpeed;
    Animation animation;
    sf::Sprite body;
    const float gravity = -825.0f;
    const float rotation = 200.0f;
    const float multiplier = ; // Making movement frame independent
};

#endif //PLAYER_H
