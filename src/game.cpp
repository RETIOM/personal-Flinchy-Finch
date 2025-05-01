//
// Created by tommeh on 04.04.25.
//

#include "../include/game.h"

#include <iostream>

Game::Game(Difficulty difficulty, Mode mode, sf::RenderWindow &window) :
groundTexture("./assets/ground.png"),
bgTexture("./assets/bg.png"),
topPipeTexture("./assets/tube1.png"), bottomPipeTexture("./assets/tube2.png"),
birdTexture("./assets/bird.png"),
_window(window),
_mode(mode),
ground(_window, groundTexture),
pipes(window, topPipeTexture, bottomPipeTexture, score),
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
            deltaSpeed = [this]() {return score*0.03f*(_window.getSize().y/400.f);};
            break;
    }
    switch (_mode) {
        case Mode::MANUAL:
            players = new PlayerManager(pipes, window, birdTexture);
            break;
        case Mode::AI:
            players = new AIManager(pipes, window, birdTexture, 150, font);
    }

    velocity = _window.getSize().x / 4;
}

void Game::update(float deltaTime) {
    if (players->getAlive()) {
        if (previousScore != score) velocity += deltaSpeed();
        previousScore = score;
        updateScore();
        players->update(deltaTime);
        ground.update(deltaTime, velocity);
        pipes.update(deltaTime, velocity);
    }
    else if (_mode == Mode::AI) reset();
    else isDone = true;
}

void Game::draw() {
    _window.draw(background);
    players->draw();
    pipes.draw();
    ground.draw();
    _window.draw(scoreText);
}

void Game::reset() {
    score = 0;
    velocity = _window.getSize().x / 4;
    isDone = false;

    ground.reset();
    pipes.reset();

    players->reset();
}

void Game::updateScore() {
    scoreText.setString(std::to_string(score));
    auto horizontalSize = scoreText.getLocalBounds().size.x;
    auto verticalSize = scoreText.getLocalBounds().size.y;

    scoreText.setOrigin(sf::Vector2f(horizontalSize / 2, verticalSize / 2));
    scoreText.setPosition(sf::Vector2f(_window.getSize().x / 2, verticalSize / 3 + _window.getSize().y / 10));
}

