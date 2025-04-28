//
// Created by tommeh on 04.04.25.
//

#ifndef PIPE_H
#define PIPE_H


#include <SFML/Graphics.hpp>

class Pipe {
public:
    Pipe(sf::RenderWindow& window, sf::Texture& topTexture, sf::Texture& bottomTexture, int &score);
    ~Pipe() = default;

    bool isOutOfBounds() const { return outOfBounds; };
    // float getThickness() const { return topPipe.getGlobalBounds().size.x; }

    // Methods for scoring below
    float getBackPosition() const {return topPipe.getGlobalBounds().size.x + topPipe.getPosition().x;}



    // Methods for collision below
    bool isPassedPlayer() const { return passedPlayer; };
    sf::FloatRect topPipeBody() { return topPipe.getGlobalBounds(); };
    sf::FloatRect bottomPipeBody() { return bottomPipe.getGlobalBounds(); };

    // Methods for AI below(topPosition getter, botPosition getter, x position getter)
    float getPosition() const { return topPipe.getPosition().x; }
    float getFrontPosition() const {return topPipe.getGlobalBounds().size.x - topPipe.getPosition().x;}
    float getTopYPosition() const {return topPipe.getPosition().y;}
    float getBottomYPosition() const {return bottomPipe.getPosition().y;}

    void update(float deltaTime, float velocity = 100.f);
    void draw() const;
private:
    int generatePipePosition();

    int pipeDistance;
    bool outOfBounds = false;
    bool passedPlayer = false;

    sf::RenderWindow& _window;
    sf::Sprite topPipe;
    sf::Sprite bottomPipe;

    int& _score;

};


#endif //PIPE_H
