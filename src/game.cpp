//
// Created by tommeh on 04.04.25.
//

#include "../include/game.h"

Game::Game(Difficulty difficulty, Mode mode, sf::RenderWindow &window) :
groundTexture("./assets/ground.png"),
bgTexture("./assets/bg.png"),
topPipeTexture("./assets/tube1.png"), bottomPipeTexture("./assets/tube2.png"),
birdTexture("./assets/bird.png"),
_window(window),
_mode(mode),
players(_mode, pipes, _window, birdTexture), pipes(_window, topPipeTexture, bottomPipeTexture, score),
ground(_window, groundTexture),
background(sf::Vector2f(_window.getSize().x, _window.getSize().y - 112.f)),
font("./assets/PressStart2P-Regular.ttf"), scoreText(font) {
    //// Create objects
    // Background
    background.setTexture(&bgTexture);
    background.setPosition(sf::Vector2f(0,0));
    // Score
    scoreText.setCharacterSize(_window.getSize().y / 10);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(scoreText.getCharacterSize() / 10);
    updateScore();

    // Handle options
    switch (difficulty) {
        case Difficulty::EASY:
            deltaSpeed = []() {return 0.f;};
            break;
        case Difficulty::HARD:
            deltaSpeed = [this]() {return score*0.5f;};
            break;
    }
}

void Game::update(float deltaTime) {
    if (players.getAlive()) {
        if (previousScore != score) velocity += deltaSpeed();
        updateScore();
        players.update(deltaTime);
        ground.update(deltaTime, velocity);
        pipes.update(deltaTime, velocity);
        previousScore = score;
    }
    else if (_mode == Mode::AI) reset();
    else isDone = true;
}

void Game::draw() {
    _window.draw(background);
    players.draw(_mode);
    pipes.draw();
    ground.draw();
    _window.draw(scoreText);
}

void Game::reset() {
    score = 0;
    isDone = false;

    ground.reset();
    pipes.reset();

    players.reset(_mode);
}

void Game::updateScore() {
    scoreText.setString(std::to_string(score));
    auto horizontalSize = scoreText.getLocalBounds().size.x;
    auto verticalSize = scoreText.getLocalBounds().size.y;

    scoreText.setOrigin(sf::Vector2f(horizontalSize / 2, verticalSize / 2));
    scoreText.setPosition(sf::Vector2f(_window.getSize().x / 2, verticalSize / 3 + _window.getSize().y / 10));
}

