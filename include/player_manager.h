//
// Created by tommeh on 20.04.25.
//

#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "player.h"
#include "pipe_manager.h"
#include "mode.h"
#include <vector>
#include <memory>

class PlayerManager {
public:
    PlayerManager(Mode mode, PipeManager &pipes, sf::RenderWindow& window, sf::Texture& birdTexture);
    ~PlayerManager() = default;
    std::size_t getAlive() {return playersAlive;};
    void update(float deltaTime);
    void draw(Mode mode);
    void reset(Mode mode);
private:
    PipeManager &_pipes;
    sf::Texture& _birdTexture;
    sf::RenderWindow &_window;
    std::vector<std::unique_ptr<Player>> players;
    Mode _mode;

    std::size_t playersAlive;
};

#endif //PLAYER_MANAGER_H
