#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Obstacle(const std::string& textureFile, float x, float y);
    void render(sf::RenderWindow& window) const; 
    sf::FloatRect getBounds() const;
};
