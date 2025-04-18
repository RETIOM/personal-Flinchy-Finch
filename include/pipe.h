//
// Created by tommeh on 04.04.25.
//

#ifndef PIPE_H
#define PIPE_H


#include <SFML/Graphics.hpp>

class Pipe {
public:
    Pipe(sf::RenderWindow& window, sf::Texture& topTexture, sf::Texture& bottomTexture);
    ~Pipe() = default;

    bool isOutOfBounds() const { return outOfBounds; };
    // float getThickness() const { return topPipe.getGlobalBounds().size.x; }

    // Methods for scoring below
    float getBackPosition() const {return topPipe.getGlobalBounds().size.x + topPipe.getPosition().x;}


    // Methods for collision below


    // Methods for AI below(topPosition getter, botPosition getter, x position getter)
    float getPosition() const { return topPipe.getPosition().x; }


    void update(float deltaTime, float velocity = 100.f);
    void draw() const;
private:
    int generatePipePosition();

    int pipeDistance;
    bool outOfBounds = false;

    sf::RenderWindow& _window;
    sf::Sprite topPipe;
    sf::Sprite bottomPipe;



};


#endif //PIPE_H
