#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(sf::Texture& texture, sf::Vector2u imageCount, float switchTime);
    ~Animation() = default;

    void update(float deltaTime, bool isFalling, bool jumped);

public:
    sf::IntRect _uvRect;

private:
    sf::Vector2u _imageCount;
    sf::Vector2u _currentImage;
    float _totalTime;
    float _switchTime;
};



#endif //ANIMATION_H
