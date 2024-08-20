#pragma once
#include <SFML/Graphics.hpp>

class PowerUp {
private:
    sf::Sprite sprite;
    sf::Vector2u textureSize;
    float speedFactor;

public:
    PowerUp();
    PowerUp(sf::Texture& powerUpTexture);
    void render(sf::RenderWindow& window) const; 
    bool update(float deltaTime);

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f);

    sf::FloatRect getBounds() const;

};
