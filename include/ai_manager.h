//
// Created by tommeh on 24.04.25.
//

#ifndef AI_MANAGER_H
#define AI_MANAGER_H
#include "agent_manager.h"
#include "player.h"
#include <vector>

class AIManager : public AgentManager {
public:
    AIManager();
    ~AIManager() override = default;

    void update(float deltaTime) override;
    void draw() override;
    void reset() override;
private:
    std::vector<Player> players;
};

#endif //AI_MANAGER_H
