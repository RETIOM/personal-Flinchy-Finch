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
    std::unordered_map<int, std::vector<std::unique_ptr<AIPlayer>>> species;

    // Divide into species
    for (auto &player : players) {
    for (auto &[num, specimen] : species) {
    if (player.compareSimilarity(*specimen[0]) < similarityThreshold) species[num].push_back(std::make_unique<AIPlayer>(player));
    }
    speciesCount++;
    species[speciesCount].push_back(std::make_unique<AIPlayer>(player));
    }

    // Recalculate fitness
}


