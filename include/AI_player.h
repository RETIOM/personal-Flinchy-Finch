//
// Created by tommeh on 28.04.25.
//

#ifndef AI_PLAYER_H
#define AI_PLAYER_H
#include "player.h"
#include "genome.h"

class AIPlayer final : public Player {
public:
  AIPlayer(PipeManager& pipes, sf::RenderWindow& window, sf::Texture& texture) : Player(pipes, window, texture), genome(inputs, outputs) {};
  AIPlayer(AIPlayer& fitter, AIPlayer& other, sf::Texture& texture) : Player(fitter._pipes, fitter._window, texture), genome(fitter.genome, other.genome) {};
  AIPlayer(const AIPlayer& parent, sf::Texture& texture) : Player(parent._pipes, parent._window, texture), genome(parent.genome) {};
  ~AIPlayer() override = default;

  void update(float deltaTime) override;

  double compareSimilarity(AIPlayer& other) {return genome.compareSimilarity(other.genome);};
  Genome genome;

  int fitnessScore = 0;

private:
  // Computes: distanceFromGround, distanceFromPipeX, distanceFromTopOfHoleY, distanceFromBottomOfHoleY, maybe ySpeed?
  std::vector<double> prepareInputs() const;
  const int inputs = 5;
  const int outputs = 1;
};

#endif //AI_PLAYER_H
