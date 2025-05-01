//
// Created by tommeh on 28.04.25.
//

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
    // std::cout<<playersAlive<<std::endl;
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
    // draw numSpecies, alive and generation
}

bool AIManager::compareFitness(AIPlayer* player1, AIPlayer* player2) {
    return player1->fitnessScore > player2->fitnessScore;
}


std::unordered_map<int, std::vector<AIPlayer*>> AIManager::speciate() {
    bool inserted;
    std::unordered_map<int, std::vector<AIPlayer*>> speciesMap;


    for (auto &player : players) {
        inserted = false;

        for (auto &[num, species] : speciesMap) {
            // std::cout<<player.compareSimilarity(*species.front())<<std::endl;
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

void AIManager::recalcuateFitness(std::unordered_map<int, std::vector<AIPlayer*>> &speciesMap) {
    for (auto& [num, species] : speciesMap) {
        int totalFitness = 0;
        for (const auto& organism : species) {
            totalFitness += organism->fitnessScore;
        }
        for (const auto& organism : species) {
            organism->fitnessScore = organism->fitnessScore / totalFitness;
        }
    }
}



AIPlayer*& AIManager::chooseFromSpecies(std::vector<AIPlayer*>& species) {
    std::vector<double> cumulativeFitness;
    cumulativeFitness.push_back(0);


    for (const auto& organism : species) {
        cumulativeFitness.push_back(organism->fitnessScore+cumulativeFitness.back());
    }

    auto totalFitness = cumulativeFitness.back();

    auto randomValue = utils::getRandom(1, totalFitness);
    // std::cout<<"TOTAL SIZE(EXP: < "<<totalFitness<<") :"<<randomValue<<std::endl;
    //
    for (int i = 0; i < cumulativeFitness.size(); i++) {
        if (randomValue < cumulativeFitness[i]) {
            // std::cout<<"FITNESS: "<<species[i-1]->fitnessScore<<std::endl;
            return species[i-1];
        }
    }
    return species[0];
}

int AIManager::chooseSpecies(std::vector<std::size_t>& cumulativeSizes, std::vector<int>& keys) {
    const auto totalSize = cumulativeSizes.back();

    const auto randomNumber = utils::getRandom(1, totalSize);
    // std::cout<<"TOTAL SIZE(EXP: < "<<totalSize<<") :"<<randomNumber<<std::endl;
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
    // Flatten speciesMap(to allow for species selection); by size
    std::vector<std::size_t> cumulativeSizes;
    std::vector<int> keys;

    cumulativeSizes.push_back(0);
    keys.push_back(-1);

    for (auto& [key, species] : speciesMap) {
        keys.push_back(key);
        cumulativeSizes.push_back(species.size()+cumulativeSizes.back());
    }


    for (int i = 0; i < number; i++) {
        if (utils::getRandom(0, 1) < infraSpeciesProb) {
            // Choose species
            auto key = chooseSpecies(cumulativeSizes, keys);

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
            auto firstKey = chooseSpecies(cumulativeSizes, keys);

            auto secondKey = chooseSpecies(cumulativeSizes, keys);

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
    std::vector<std::size_t> cumulativeSizes;
    std::vector<int> keys;

    cumulativeSizes.push_back(0);
    keys.push_back(-1);

    for (auto& [key, species] : speciesMap) {
        keys.push_back(key);
        cumulativeSizes.push_back(species.size()+cumulativeSizes.back());
    }

    for (int i = 0; i < number; i++) {
        auto key = chooseSpecies(cumulativeSizes, keys);
        auto player = chooseFromSpecies(speciesMap.at(key));

        // newPlayers.emplace_back(*player, _birdtexture, true);
        // std::cout<<player->fitnessScore<<std::endl;
        newPlayers.emplace_back(*player, _birdtexture, false);
    }
}




// Handles selection and breeding, consider helper functions(split into helpers perhaps)
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
    // std::cout<<speciesMap.size()<<std::endl;

    // Recalculate fitness
    recalcuateFitness(speciesMap);

    // Fill new generation
    int counter = 0;

    // Move fittest
    for (auto& [num, species] : speciesMap) {
        if (species.size() > 5) {
            newPlayers.emplace_back(*species[0], _birdtexture);
            // std::cout<<species[0]->fitnessScore<<std::endl;
            // std::cout<<species[149]->fitnessScore<<std::endl;
            counter++;
        }
    }

    const int withoutCrossover = static_cast<int>(0.25*_maxPlayers);
    const int withCrossover = _maxPlayers-counter-withoutCrossover;

    // Mutate/Crossover
    mutate(newPlayers, speciesMap, withoutCrossover);
    crossover(newPlayers, speciesMap, withCrossover);


    // std::cout<<newPlayers.size()<<std::endl;
    players.clear();
    players = std::move(newPlayers);
}


