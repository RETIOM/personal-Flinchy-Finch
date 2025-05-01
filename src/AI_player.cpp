#include "../include/AI_player.h"


void AIPlayer::update(float deltaTime) {
    if (!isAlive) return;

    fitnessScore += deltaTime*100;
    sf::Vector2f movement(0.0f, 0.0f);

    bool networkJump;
    genome.getOutput(prepareInputs())[0] > 0.5 ? networkJump = true : networkJump = false;
    if (networkJump) {
        ySpeed = jumpSpeed;
    }

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

std::vector<double> AIPlayer::prepareInputs() const {
    std::vector<double> inputs;

    auto agentPosition = body.getPosition();
    Pipe& nextPipe = _pipes.nextPipe();

    // inputs.push_back((_window.getSize().y - agentPosition.y-112.f)/_window.getSize().y); // Distance from ground
    inputs.push_back((nextPipe.getFrontPosition() - agentPosition.x)/_window.getSize().x); // Distance from pipe
    inputs.push_back((nextPipe.getTopYPosition() - agentPosition.y)/_window.getSize().y); // Distance from top of hole
    inputs.push_back((agentPosition.y - nextPipe.getBottomYPosition() )/_window.getSize().y); // Distance from bottom of pipe
    // inputs.push_back(ySpeed/maxSpeed); // Vertical velocity

    return inputs;
}






