//
// Created by tommeh on 28.04.25.
//

#include "../include/AI_manager.h"
#include <unordered_map>
#include <memory>

AIManager::AIManager(PipeManager &pipes, sf::RenderWindow &window, sf::Texture &birdTexture, std::size_t maxPlayers) :
_window(window),
_birdtexture(birdTexture),
_pipes(pipes) {
    for (std::size_t i = 0; i < maxPlayers; i++) {
        players.emplace_back(_pipes, _window, _birdtexture);
    }
}

void AIManager::update(float deltaTime) {
    for (auto &player : players) {
        player.update(deltaTime);
    }
}

void AIManager::draw() {
    constexpr int maxDraw = 20;
    int counter = 1;

    for (auto &player : players) {
        if (counter > maxDraw) {break;}
        if (player.alive()) {
            player.draw();
            counter++;
        }
    }
}


// Handles selection and breeding, consider helper functions(split into helpers perhaps)
void AIManager::reset() {
    int speciesCount = 0;
    std::unordered_map<int, std::vector<std::unique_ptr<AIPlayer>>> speciesMap;
    std::vector<AIPlayer> newPlayers;

    // Divide into species
    for (auto &player : players) {
        for (auto &[num, species] : speciesMap) {
            if (player.compareSimilarity(*species[0]) < similarityThreshold) {
                speciesMap[num].push_back(std::make_unique<AIPlayer>(player));
            }
        }
        speciesCount++;
        speciesMap[speciesCount].push_back(std::make_unique<AIPlayer>(player));
    }

    // Recalculate fitness
    for (auto& [num, species] : speciesMap) {
        int totalFitness = 0;
        for (const auto& organism : species) {
            totalFitness += organism->fitnessScore;
        }
        for (auto& organism : species) {
            organism->fitnessScore /= totalFitness;
        }
        std::ranges::sort(species); // Create a comparision function for fitnessScore
    }

    // Start creating new generation
    int counter = 0;

    // Move fittest
    for (auto& [num, species] : speciesMap) {
        if (species.size() > 5) {
            newPlayers.emplace_back(*species[0], _birdtexture);
            counter++;
        }
    }

    while (counter < maxPlayers) {

    }
    // Move without breeding(just mutation) (for i in range 0.25maxPlayers) choose
    // Or make it a probability game :) <- better cause I can merge into one function

    // Breed(with a chance of interSpecies)
}


