//
// Created by tommeh on 04.04.25.
//

#ifndef GAME_H
#define GAME_H
#include "difficulty.h"
#include "mode.h"
#include "player_manager.h"
#include "pipe_manager.h"
#include "ground.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Game {
public:
    Game(Difficulty difficulty, Mode mode, sf::RenderWindow& window);
    ~Game() = default;
    void update(float deltaTime);
    void draw();
    void reset();
    bool getIsDone() const {return isDone;};
private:
    float velocity = 100.f;
    int score = 0;
    int previousScore = 0;
    bool isDone = false;
    std::function<float()> deltaSpeed;

    sf::Texture groundTexture;
    sf::Texture bgTexture;
    sf::Texture topPipeTexture;
    sf::Texture bottomPipeTexture;
    sf::Texture birdTexture;

    sf::RenderWindow& _window;

    Mode _mode;
    PlayerManager players;
    PipeManager pipes;
    Ground ground;


    sf::RectangleShape background;
    sf::Font font;
    sf::Text scoreText;
};


#endif //GAME_H
