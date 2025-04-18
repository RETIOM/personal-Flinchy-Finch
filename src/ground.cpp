//
// Created by tommeh on 17.04.25.
//
#include "../include/ground.h"

#include <iostream>

Ground::Ground(sf::RenderWindow &window, sf::Texture &texture) : _window(window), _ground1(texture), _ground2(texture) {
    const auto scale = sf::Vector2f(_window.getSize().x / _ground1.getLocalBounds().size.x,1.f);

    _ground1.setScale(scale);
    _ground2.setScale(scale);

    _ground1.setPosition(sf::Vector2f(0, _window.getSize().y - _ground1.getLocalBounds().size.y));
    _ground2.setPosition(sf::Vector2f(_window.getSize().x, _window.getSize().y - _ground2.getLocalBounds().size.y));
}
void Ground::update(const float deltaTime, const float velocity) {
    sf::Vector2f movement(0.0f, 0.0f);

    movement.x -= velocity * deltaTime;

    _ground1.move(movement);
    _ground2.move(movement);

    if (_ground1.getPosition().x + _ground1.getGlobalBounds().size.x <= 0) {
        _ground1.setPosition(sf::Vector2f(_ground2.getPosition().x + _window.getSize().x, _window.getSize().y - _ground1.getLocalBounds().size.y));
    }

    if (_ground2.getPosition().x + _ground2.getGlobalBounds().size.x <= 0) {
        _ground2.setPosition(sf::Vector2f(_ground1.getPosition().x + _window.getSize().x, _window.getSize().y - _ground2.getLocalBounds().size.y));
    }
}

void Ground::draw() const {
    _window.draw(_ground1);
    _window.draw(_ground2);
}