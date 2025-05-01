#ifndef AI_MANAGER_H
#define AI_MANAGER_H
#include "agent_manager.h"
#include "player.h"
#include "AI_player.h"
#include <vector>


class AIManager : public AgentManager {
public:
    AIManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture, std::size_t maxPlayers, sf::Font &font);
    ~AIManager() override = default;

    void update(float deltaTime) override;
    void draw() override;
    void reset() override;

    std::size_t getAlive() {return playersAlive;}
private:
    sf::Font& _font;

    const int _maxPlayers;
    std::size_t playersAlive = _maxPlayers;

    int generation = 0;

    std::vector<AIPlayer> players;
    sf::RenderWindow &_window;
    sf::Texture &_birdtexture;
    PipeManager &_pipes;

    const double similarityThreshold = 3;

    // Speciating functions
    int speciesCount = 0;

    static bool compareFitness(AIPlayer* player1, AIPlayer* player2);
    std::unordered_map<int, std::vector<AIPlayer*>> speciate();

    double sumAdjustedFitness(std::vector<AIPlayer*>& players);
    std::pair<std::vector<double>, std::vector<int>> flattenSpecies(std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap);

    void recalculateFitness(std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap);


    // Breeding functions
    void moveFittest(std::vector<AIPlayer>& newPlayers, std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap);


    AIPlayer*& chooseFromSpecies(std::vector<AIPlayer*>& species);
    int chooseSpecies(std::vector<double>& cumulativeSizes, std::vector<int>& keys);
    void crossover(std::vector<AIPlayer>& newPlayers, std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap, int number);

    void mutate(std::vector<AIPlayer>& newPlayers, std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap, int number);

    // Look good functions
    float getMaxFitness(std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap);
};

#endif //AI_MANAGER_H
