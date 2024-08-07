#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Obstacle.h"
#include "PowerUp.h"

class Car {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float lanePosition;

public:
    Car(const std::string& textureFile);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void moveLeft();
    void moveRight();
    sf::FloatRect getBounds() const;
    void checkCollision(const std::vector<Obstacle>& obstacles);
    void checkCollision(const std::vector<PowerUp>& powerUps);
};
