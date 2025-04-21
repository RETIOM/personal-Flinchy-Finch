//
// Created by tommeh on 18.04.25.
//

#ifndef PIPE_CONTROLLER_H
#define PIPE_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "pipe.h"
#include <deque>

class PipeManager {
public:
    PipeManager(sf::RenderWindow &window, sf::Texture &topPipeTexture, sf::Texture &bottomPipeTexture, int& score);
    ~PipeManager() = default;
    void update(float deltaTime, float velocity = 100.f);
    void draw();
    void reset();

    // Methods for collision
    Pipe& nextPipe();
private:
    std::deque<Pipe> pipes;
    float pipeHorizontalDistance;
    sf::Texture& _topPipeTexture;
    sf::Texture& _bottomPipeTexture;
    sf::RenderWindow& _window;
    int& _score;
};

#endif //PIPE_CONTROLLER_H
