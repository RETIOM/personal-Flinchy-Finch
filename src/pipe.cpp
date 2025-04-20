//
// Created by tommeh on 04.04.25.
//

#include "../include/pipe.h"

#include <random>
#include <algorithm>
#include <iostream>

Pipe::Pipe(sf::RenderWindow &window, sf::Texture &topTexture, sf::Texture &bottomTexture, int &score) : _window(window), topPipe(topTexture), bottomPipe(bottomTexture), _score(score) {
    const auto scale = (window.getSize().y - 112.f) / topPipe.getLocalBounds().size.y;

    topPipe.setOrigin(sf::Vector2f(0, topPipe.getGlobalBounds().size.y));
    bottomPipe.setOrigin(sf::Vector2f(0, 0));

    topPipe.setScale(sf::Vector2f(scale, scale));
    bottomPipe.setScale(sf::Vector2f(scale, scale));

    pipeDistance = 25*(_window.getSize().y - 112.f) / 100;

    const int pipePosition = generatePipePosition();

    topPipe.setPosition(sf::Vector2f(_window.getSize().x, pipePosition));
    bottomPipe.setPosition(sf::Vector2f(_window.getSize().x, pipePosition+pipeDistance));
}


int Pipe::generatePipePosition() {
    const int min = 50 * _window.getSize().y / 600u; // Scale with resolution
    const int max = _window.getSize().y - 112.f - pipeDistance - min;
    const auto mean = (min+max) / 2;

    const int stddev = _window.getSize().y / 3;

    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::normal_distribution<double> dist(mean, stddev);

    const double value = dist(rng);
    return std::clamp(static_cast<int>(std::floor(value)), min, max);
}

void Pipe::update(float deltaTime, float velocity) {
    sf::Vector2f movement(0.0f, 0.0f);

    movement.x -= velocity * deltaTime;

    topPipe.move(movement);
    bottomPipe.move(movement);

    if (passedPlayer && this->getBackPosition() <= 0) {outOfBounds = true;};
    if (!passedPlayer && this->getBackPosition() <= _window.getSize().x / 3) {passedPlayer = true; _score++;};
}


void Pipe::draw() const {
    _window.draw(topPipe);
    _window.draw(bottomPipe);
}





