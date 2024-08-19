#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite obstacleSprite;
    bool isOffScreen = false;
    sf::Vector2u textureSize;

public:
    Obstacle(sf::Texture &obstacleTexture);
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const; 
    sf::FloatRect getBounds() const;
    bool ifOffScreen() const;
};
