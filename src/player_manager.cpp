//
// Created by tommeh on 20.04.25.
//

#include "../include/player_manager.h"

#include <iostream>

PlayerManager::PlayerManager(Mode mode, PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture) : _pipes(pipes), _window(window), _birdTexture(birdTexture),  _mode(mode) {
    playersAlive = 1;
    players.emplace_back(std::make_unique<Player>(_pipes,_window,_birdTexture));
    //// FOR AI MODE
    // for (int i = 0; i < maxPlayers; i++) {
    //     players.emplace_back(std::make_unique<Player>(_pipes,_window,_birdTexture));
    // }
}

void PlayerManager::update(float deltaTime) {
    for (auto &player : players) {
        player->update(deltaTime);
        if (!player->alive()) {
            playersAlive--;
            if (playersAlive > 1) {continue;}
        }
    }
}

void PlayerManager::draw(Mode mode) {
    if (mode == Mode::MANUAL) {
        for (auto &player : players) {player->draw();}
    }
    else {
        for (auto &player : players) {
            if (player->alive()) {
                player->draw();
            }
        }
    }
}

void PlayerManager::reset(Mode mode) {
    if (mode == Mode::MANUAL) {
        for (auto &player : players) {
            player->reset();
        }
        playersAlive = 1;
    }

}


