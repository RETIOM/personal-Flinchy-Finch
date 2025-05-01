#ifndef AGENT_MANAGER_H
#define AGENT_MANAGER_H
#include <cstddef>

class AgentManager {
public:
    virtual ~AgentManager() = default;

    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
    virtual void reset() = 0;
    virtual std::size_t getAlive() = 0;
};

#endif //AGENT_MANAGER_H
