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

    // sf::IntRect getRect();
    bool isOutOfBounds() { return outOfBounds; };

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
