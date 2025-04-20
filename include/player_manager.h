//
// Created by tommeh on 20.04.25.
//

#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "player.h"
#include "pipe_manager.h"
#include <vector>
#include <memory>

class playerManager {
public:
    playerManager(PipeManager &pipes, sf::RenderWindow& window, sf::Texture& birdTexture, std::size_t maxPlayers);
    ~playerManager() = default;
    std::size_t getAlive() {return playersAlive;};
    void update(float deltaTime);
private:
    PipeManager &_pipes;
    sf::Texture& _birdTexture;
    sf::RenderWindow &_window;
    std::vector<std::unique_ptr<Player>> players;

    std::size_t playersAlive;
};

#endif //PLAYER_MANAGER_H
