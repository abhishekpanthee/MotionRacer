#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite obstacleSprite;
    bool isOffScreen = false;
    sf::Vector2u textureSize;
    float speedFactor;

public:
    Obstacle(sf::Texture &obstacleTexture);
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const; 

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f);

    sf::FloatRect getBounds() const;
    bool ifOffScreen() const;
};
