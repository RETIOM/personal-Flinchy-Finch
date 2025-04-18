//
// Created by tommeh on 18.04.25.
//
#include "../include/pipe_controller.h"


PipeController::PipeController(sf::RenderWindow &window, sf::Texture &topPipeTexture, sf::Texture &bottomPipeTexture) : _window(window), _topPipeTexture(topPipeTexture), _bottomPipeTexture(bottomPipeTexture) {
    pipes.emplace_back(Pipe(_window, _topPipeTexture, _bottomPipeTexture));

    pipeHorizontalDistance = _window.getSize().x * 0.25f;
}

void PipeController::update(float deltaTime, float velocity) {
    if (pipes.front().isOutOfBounds()) {pipes.pop_front();}
    if (_window.getSize().x - pipes.back().getBackPosition() >= pipeHorizontalDistance) {
        pipes.emplace_back(Pipe(_window, _topPipeTexture, _bottomPipeTexture));
    }

    for (auto &pipe : pipes) {
        pipe.update(deltaTime, velocity);
        pipe.draw();
    }
}

