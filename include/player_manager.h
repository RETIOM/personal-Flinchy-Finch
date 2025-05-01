#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include "agent_manager.h"
#include "player.h"
#include "pipe_manager.h"

class PlayerManager : public AgentManager {
public:
    PlayerManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture) : _pipes(pipes), _window(window), player(pipes, window, birdTexture){}
    ~PlayerManager() override = default;
    std::size_t getAlive() override {return player.alive();};
    void update(float deltaTime) override {player.update(deltaTime);};
    void draw() override {player.draw();};
    void reset() override {player.reset();};
private:
    PipeManager &_pipes;
    sf::RenderWindow &_window;

    Player player;
};

#endif //PLAYER_MANAGER_H
