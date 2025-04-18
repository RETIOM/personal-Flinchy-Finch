//
// Created by tommeh on 17.04.25.
//

#include "../include/animation.h"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime) {
    _switchTime = switchTime;
    _imageCount = imageCount;
    _totalTime = 0.0f;
    _currentImage.x = 0;
    _currentImage.y = 0;

    _uvRect.size.x = texture->getSize().x / imageCount.x;
    _uvRect.size.y = texture->getSize().y / imageCount.y;
}

void Animation::update(const float deltaTime, bool isFalling) {
    _totalTime += deltaTime;
    if (!isFalling) {
        if (_totalTime >= _switchTime) {
            _totalTime -= _switchTime;
            _currentImage.x++;

            if (_currentImage.x >= _imageCount.x) {
                _currentImage.x = 0;
            }
        }
        _uvRect.position.x = _currentImage.x * _uvRect.size.x;
    }
    else {
        _uvRect.position.x = 1 * _uvRect.size.x; // If facing down (ANGLE WILL BE BASED ON VELOCITY) then dont flap wings
    }
}


