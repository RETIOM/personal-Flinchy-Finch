//
// Created by tommeh on 20.04.25.
//

#include "../include/player_manager.h"

playerManager::playerManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture, std::size_t maxPlayers) : _pipes(pipes), _window(window), _birdTexture(birdTexture), playersAlive(maxPlayers) {
    for (int i = 0; i < maxPlayers; i++) {
        players.emplace_back(std::make_unique<Player>(_pipes,_window,_birdTexture));
    }
}

void playerManager::update(float deltaTime) {
    for (auto &player : players) {
        player->update(deltaTime);
        if (!player->alive()) {
            playersAlive--;
            if (playersAlive > 1) {continue;}
        }
        player->draw();
    }
}

