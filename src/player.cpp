//
// Created by tommeh on 04.04.25.
//


#include "../include/player.h"

#include <iostream>

Player::Player(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &texture) : animation(texture, sf::Vector2u(3,1), 0.1f), body(texture), _window(window), _pipes(pipes) {
    angle = sf::degrees(0);
    ySpeed = 0;


    // Set bird position first, to make sure it's BACK is at the 1/3 of screen
    unsigned int horizontalSize = window.getSize().x;
    unsigned int verticalSize = window.getSize().y;


    // Set origin to bird center to make sure rotation and scale works good
    body.setOrigin(sf::Vector2f(texture.getSize().x / 6, texture.getSize().y / 2));

    auto scale = (_window.getSize().y * 0.07f) / body.getLocalBounds().size.y;
    body.setScale(sf::Vector2f(scale, scale));

    size = sf::Vector2f((texture.getSize().x / 3) * scale, texture.getSize().y * scale);

    body.setPosition(sf::Vector2f(horizontalSize / 3 + 1.01*size.x / 2, verticalSize / 2));

    gravity *= verticalSize / 600;
    jumpSpeed *= verticalSize / 600;
    rotation *= verticalSize / 600;
}

void Player::update(float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    if (!isAlive) {
        body.setColor(sf::Color::Red);
        return;
    }



    const bool isJumpKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (isJumpKeyDown && !hasJumped) {
        ySpeed = jumpSpeed;
    }
    hasJumped = isJumpKeyDown;

    movement.y += ySpeed * deltaTime;

    if (ySpeed < 0) {
        angle = sf::degrees(-30);
    } else {
        angle = std::min(sf::degrees(90), angle + sf::degrees(rotation) * deltaTime);
    }

    if (std::abs(ySpeed) < maxSpeed) ySpeed -= gravity * deltaTime;

    body.move(movement);


    body.setRotation(angle);

    animation.update(deltaTime, angle == sf::degrees(90), hasJumped);
    body.setTextureRect(animation._uvRect);
    this->checkCollision();
}


void Player::draw() const {
    _window.draw(body);
}

void Player::checkCollision() {
    Pipe& nextPipe = _pipes.nextPipe();
    sf::FloatRect topPipeHitbox = nextPipe.topPipeBody();
    sf::FloatRect bottomPipeHitbox = nextPipe.bottomPipeBody();

    // Creating own hitbox bc getGlobalBounds() blows :)
    auto birdPosition = body.getPosition();
    birdPosition.x -= size.x / 2;
    birdPosition.y -= size.y / 2;
    auto hitbox = sf::FloatRect(birdPosition, size);
    auto birdBottom = birdPosition.y + (size.y / 2);
    auto birdRight = birdPosition.x + (size.x / 2);

    // std::cout<<topPipeHitbox.position.x<<std::endl;

    if (hitbox.findIntersection(topPipeHitbox) || hitbox.findIntersection(bottomPipeHitbox)) {
        isAlive = false;
    }
    else if (birdBottom >= _window.getSize().y-112.f) {
        isAlive = false;
    }
    else if (birdBottom <= 0 && birdRight >= topPipeHitbox.position.x) {
        isAlive = false;
    }
    if (!isAlive) {body.setColor(sf::Color::Red);}
}

void Player::reset() {
    angle = sf::degrees(0);
    ySpeed = 0;
    body.setPosition(sf::Vector2f(_window.getSize().x / 3 + 1.01*size.x / 2, _window.getSize() .y/ 2));
    body.setColor(sf::Color::White);
    isAlive = true;

}