//
// Created by tommeh on 04.04.25.
//

#include "../include/player.h"

Player::Player(sf::RenderWindow &window, sf::Texture &texture, sf::Vector2u imageCount, float switchTime) : animation(texture, imageCount,
    switchTime), body(texture), _window(window) {
    angle = sf::degrees(0);
    ySpeed = 0;
    body.setOrigin(sf::Vector2f(texture.getSize().x / 6, texture.getSize().y / 2));

    body.setScale(sf::Vector2f(1.5f, 1.5f));

    unsigned int horizontalSize = window.getSize().x;
    unsigned int verticalSize = window.getSize().y;

    body.setPosition(sf::Vector2f(horizontalSize / 3, verticalSize / 2));
    gravity *= verticalSize / 600;
    jumpSpeed *= verticalSize / 600;
    rotation *= verticalSize / 600;
}

void Player::update(float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    // if (!isAlive && !) {
    //
    //     return;
    // }



    const bool isJumpKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (isJumpKeyDown && !wasSpacePressed) {
        ySpeed = jumpSpeed;
    }
    wasSpacePressed = isJumpKeyDown;

    movement.y += ySpeed * deltaTime;

    if (ySpeed < 0) {
        angle = sf::degrees(-30);
    } else {
        angle = std::min(sf::degrees(90), angle + sf::degrees(rotation) * deltaTime);
    }

    ySpeed -= gravity * deltaTime;

    body.move(movement);

    // if (body.getGlobalBounds().findIntersection(.))

    body.setRotation(angle);

    animation.update(deltaTime, angle == sf::degrees(90), wasSpacePressed);
    body.setTextureRect(animation._uvRect);
}


void Player::draw() const {
    _window.draw(body);
}

bool Player::hasCollided() {
    return false;
}

