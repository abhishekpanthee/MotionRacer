#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    PowerUp(const std::string& textureFile, float x, float y);
    void render(sf::RenderWindow& window) const; 
    sf::FloatRect getBounds() const;
};
