//
// Created by tommeh on 24.04.25.
//

#ifndef AI_MANAGER_H
#define AI_MANAGER_H
#include "agent_manager.h"
#include "player.h"
#include "AI_player.h"
#include <vector>


class AIManager : public AgentManager {
public:
    AIManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture, std::size_t maxPlayers);
    ~AIManager() override = default;

    void update(float deltaTime) override;
    void draw() override;
    void reset() override;
private:
    std::vector<AIPlayer> players;
    sf::RenderWindow &_window;
    sf::Texture &_birdtexture;
    PipeManager &_pipes;

    const double similarityThreshold = 3.0;
};

#endif //AI_MANAGER_H
