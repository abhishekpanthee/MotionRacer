#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
private:
    sf::Sprite sprite;
    bool isOffScreen;
    sf::Vector2u textureSize;

public:
    PowerUp(sf::Texture& powerUpTexture);
    void render(sf::RenderWindow& window) const; 
    void update(float deltaTime);
    sf::FloatRect getBounds() const;

};
