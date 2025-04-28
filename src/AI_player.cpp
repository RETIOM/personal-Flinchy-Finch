//
// Created by tommeh on 28.04.25.
//

#include "../include/AI_player.h"


void AIPlayer::update(float deltaTime) {
    if (!isAlive) return;

    fitnessScore += deltaTime;
    sf::Vector2f movement(0.0f, 0.0f);

    bool networkJump;
    // Output will be a vector with one value(since one output)
    genome.getOutput(prepareInputs())[0] > 0 ? networkJump = true : networkJump = false;
    if (networkJump && !hasJumped) {
        ySpeed = jumpSpeed;
    }
    hasJumped = networkJump;

    movement.y += ySpeed * deltaTime;

    if (ySpeed < 0) {
        angle = sf::degrees(-30);
    } else {
        angle = std::min(sf::degrees(90), angle + sf::degrees(rotation) * deltaTime);
    }

    ySpeed -= gravity * deltaTime;

    body.move(movement);


    body.setRotation(angle);

    animation.update(deltaTime, angle == sf::degrees(90), hasJumped);
    body.setTextureRect(animation._uvRect);
    this->checkCollision();
}

std::vector<double> AIPlayer::prepareInputs() const {
    std::vector<double> inputs;

    auto agentPosition = body.getPosition();
    Pipe& nextPipe = _pipes.nextPipe();

    inputs.push_back(_window.getSize().y - agentPosition.y-112.f); // Distance from ground
    inputs.push_back(nextPipe.getFrontPosition() - agentPosition.x); // Distance from pipe
    inputs.push_back(nextPipe.getTopYPosition() - agentPosition.y); // Distance from top of hole
    inputs.push_back(nextPipe.getBottomYPosition() - agentPosition.y); // Distance from bottom of pipe
    inputs.push_back(ySpeed); // Vertical velocity

    return inputs;
}






