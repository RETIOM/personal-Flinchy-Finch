//
// Created by tommeh on 04.04.25.
//

#include "../include/player.h"

Player::Player(sf::Texture *texture, sf::Vector2u imageCount, float switchTime) : animation(texture, imageCount,
    switchTime), body(*texture) {
    angle = sf::degrees(0);
    ySpeed = 0;
    body.setOrigin(sf::Vector2f(texture->getSize().x / 6, texture->getSize().y / 2));
    body.setPosition(sf::Vector2f(200.0f, 200.0f));
    body.setScale(sf::Vector2f(1.5f, 1.5f));
}

void Player::update(float deltaTime) {

    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        ySpeed = -250.0f;
    }

    movement.y += ySpeed * deltaTime;

    if (ySpeed < 0) {
        angle = sf::degrees(-30);
    } else {
        angle = std::min(sf::degrees(90), angle + sf::degrees(rotation) * deltaTime);
    }

    ySpeed -= gravity * deltaTime;

    body.move(movement);
    body.setRotation(angle);

    animation.update(deltaTime, angle == sf::degrees(90));
    body.setTextureRect(animation._uvRect);
}


void Player::draw(sf::RenderWindow &window) const {
    window.draw(body);
}


