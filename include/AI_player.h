#ifndef AI_PLAYER_H
#define AI_PLAYER_H
#include "player.h"
#include "genome.h"
#include <iostream>

class AIPlayer final : public Player {
public:
  AIPlayer(PipeManager& pipes, sf::RenderWindow& window, sf::Texture& texture) : Player(pipes, window, texture), genome(3, 1) {};
  AIPlayer(AIPlayer& fitter, AIPlayer& other, sf::Texture& texture) : Player(fitter._pipes, fitter._window, texture), genome(fitter.genome, other.genome) {};
  AIPlayer(AIPlayer& parent, sf::Texture& texture, bool mutate = false) : Player(parent._pipes, parent._window, texture), genome(parent.genome) {if (mutate) genome.runMutate();};
  ~AIPlayer() override = default;

  void update(float deltaTime) override;

  double compareSimilarity(AIPlayer& other) {return genome.compareSimilarity(other.genome);};
  Genome genome;

  float fitnessScore = 0;

private:
  // Computes: distanceFromGround, distanceFromPipeX, distanceFromTopOfHoleY, distanceFromBottomOfHoleY, maybe ySpeed?
  std::vector<double> prepareInputs() const;
};

#endif //AI_PLAYER_H
