#include "../include/AI_manager.h"
#include <unordered_map>
#include <memory>

#include "utils.h"

AIManager::AIManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture, std::size_t maxPlayers, sf::Font &font) :
_window(window),
_birdtexture(birdTexture),
_pipes(pipes),
_maxPlayers(maxPlayers),
_font(font) {
    for (std::size_t i = 0; i < maxPlayers; i++) {
        players.emplace_back(_pipes, _window, _birdtexture);
    }
}

void AIManager::update(float deltaTime) {
    playersAlive = 0;
    for (auto &player : players) {
        if (player.alive()) {
            player.update(deltaTime);
            playersAlive++;
        }
    }
}

void AIManager::draw() {
    constexpr int maxDraw = 150;
    int counter = 1;

    for (auto &player : players) {
        if (counter > maxDraw) {break;}
        if (player.alive()) {
            player.draw();
            counter++;
        }
    }
    // TODO: draw numSpecies, alive and generation + fitnessScore
}

bool AIManager::compareFitness(AIPlayer* player1, AIPlayer* player2) {
    return player1->fitnessScore > player2->fitnessScore;
}


std::unordered_map<int, std::vector<AIPlayer*>> AIManager::speciate() {
    std::unordered_map<int, std::vector<AIPlayer*>> speciesMap;


    for (auto &player : players) {
        bool inserted = false;

        for (auto &[num, species] : speciesMap) {
            if (player.compareSimilarity(*species[0]) < similarityThreshold) {
                speciesMap[num].push_back(&player);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            speciesCount++;
            speciesMap[speciesCount].push_back(&player);
        }
    }

    for (auto &[num, species] : speciesMap) {
        std::sort(species.begin(), species.end(), compareFitness);
    }
    return speciesMap;
}

void AIManager::recalculateFitness(std::unordered_map<int, std::vector<AIPlayer*>> &speciesMap) {
    for (auto& [num, species] : speciesMap) {
        for (const auto& organism : species) {
            organism->fitnessScore /= species.size();
        }
    }
}

double AIManager::sumAdjustedFitness(std::vector<AIPlayer*>& players) {
    double totalFitness = 0;
    for (const auto& organism : players) {
        totalFitness += organism->fitnessScore;
    }
    return totalFitness;
}

std::pair<std::vector<double>, std::vector<int>> AIManager::flattenSpecies(std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap) {
    std::vector<double> cumulativeAdjustedFitness;
    std::vector<int> keys;

    cumulativeAdjustedFitness.push_back(0);
    keys.push_back(-1);

    for (auto& [key, species] : speciesMap) {
        keys.push_back(key);

        //// Switch this to enable/disable innovation protection(also have to disable adjustFitness)
        cumulativeAdjustedFitness.push_back(sumAdjustedFitness(species)+cumulativeAdjustedFitness.back());
        // cumulativeAdjustedFitness.push_back(species[0]->fitnessScore+cumulativeAdjustedFitness.back());
    }

    return std::make_pair(cumulativeAdjustedFitness, keys);
}



AIPlayer*& AIManager::chooseFromSpecies(std::vector<AIPlayer*>& species) {
    std::vector<double> cumulativeFitness;
    cumulativeFitness.push_back(0);


    for (const auto& organism : species) {
        cumulativeFitness.push_back(organism->fitnessScore+cumulativeFitness.back());
    }

    auto totalFitness = cumulativeFitness.back();

    auto randomValue = utils::getRandom(1, totalFitness);

    for (int i = 0; i < cumulativeFitness.size(); i++) {
        if (randomValue < cumulativeFitness[i]) {
            return species[i-1]; // i-1 becauase lack of sentinel
        }
    }
    return species[0];
}

int AIManager::chooseSpecies(std::vector<double>& cumulativeSizes, std::vector<int>& keys) {
    const auto totalSize = cumulativeSizes.back();

    const auto randomNumber = utils::getRandom(1, totalSize);
    for (int i = 1; i < cumulativeSizes.size(); i++) {
        if (randomNumber < cumulativeSizes[i]) {
            return keys[i];
        }
    }
    return -1;
}

void AIManager::crossover(std::vector<AIPlayer>& newPlayers, std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap, int number) {
    // Choose two players(higher fitness higher chance), remember to include a chance of interspecies
    const float infraSpeciesProb = 0.999;

    auto flattenedSpeciesMap = flattenSpecies(speciesMap);

    auto cumulativeFitness = flattenedSpeciesMap.first;
    auto keys = flattenedSpeciesMap.second;


    for (int i = 0; i < number; i++) {
        if (utils::getRandom(0, 1) < infraSpeciesProb) {
            // Choose species
            auto key = chooseSpecies(cumulativeFitness, keys);

            // Choose from species
            auto player1 = chooseFromSpecies(speciesMap.at(key));
            auto player2 = chooseFromSpecies(speciesMap.at(key));

            if (player1->fitnessScore > player2->fitnessScore) {
                newPlayers.emplace_back(*player1, *player2, _birdtexture);
            }
            else {
                newPlayers.emplace_back(*player2, *player1, _birdtexture);
            }
        }
        else {
            auto firstKey = chooseSpecies(cumulativeFitness, keys);

            auto secondKey = chooseSpecies(cumulativeFitness, keys);

            auto player1 = chooseFromSpecies(speciesMap.at(firstKey));
            auto player2 = chooseFromSpecies(speciesMap.at(secondKey));

            if (player1->fitnessScore > player2->fitnessScore) {
                newPlayers.emplace_back(*player1, *player2, _birdtexture);
            }
            else {
                newPlayers.emplace_back(*player2, *player1, _birdtexture);
            }
        }
    }
}

void AIManager::mutate(std::vector<AIPlayer>& newPlayers, std::unordered_map<int, std::vector<AIPlayer*>>& speciesMap, int number) {
    auto flattenedSpeciesMap = flattenSpecies(speciesMap);

    auto cumulativeFitness = flattenedSpeciesMap.first;
    auto keys = flattenedSpeciesMap.second;

    for (int i = 0; i < number; i++) {
        auto key = chooseSpecies(cumulativeFitness, keys);
        auto player = chooseFromSpecies(speciesMap.at(key));

        newPlayers.emplace_back(*player, _birdtexture, true);
    }
}

float AIManager::getMaxFitness(std::unordered_map<int, std::vector<AIPlayer *> > &speciesMap) {
    float maxFitness = 0;
    for (auto& [key, species] : speciesMap) {
        if (species[0]->fitnessScore > maxFitness) maxFitness = species[0]->fitnessScore;
    }
    return maxFitness;
}



void AIManager::reset() {
    // Reset necessary values
    speciesCount = 0;
    playersAlive = _maxPlayers;
    generation++;

    //// Create new generation
    std::vector<AIPlayer> newPlayers;
    newPlayers.reserve(_maxPlayers);

    // Divide into species
    auto speciesMap = speciate();
    speciesCount = speciesMap.size();

    // Provide information on last gen
    printf("Generation %d: # of species %d; Max fitness score: %.2f\n", generation, speciesCount, getMaxFitness(speciesMap));

    // Recalculate fitness
    recalculateFitness(speciesMap);

    // Fill new generation
    int counter = 0;

    // Move fittest
    for (auto& [num, species] : speciesMap) {
        if (species.size() > 5) {
            newPlayers.emplace_back(*species[0], _birdtexture);
            counter++;
        }
    }

    const int withoutCrossover = static_cast<int>(0.25*_maxPlayers);
    const int withCrossover = _maxPlayers-counter-withoutCrossover;

    // Mutate/Crossover
    mutate(newPlayers, speciesMap, withoutCrossover);
    crossover(newPlayers, speciesMap, withCrossover);

    players.clear();
    players = std::move(newPlayers);
}


