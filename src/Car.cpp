#include "Car.h"
#include <SFML/Window.hpp>
#include <iostream>

Car::Car(const std::string& textureFile) : lanePosition(1) {
      // image ko directory texture file ma laijane 
    std::string fullPath = "assets/" + textureFile;
//eroor handling
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Error loading texture from " << fullPath << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(400.f, 500.f); 
}

void Car::update(float deltaTime) {
    // Rhythem, lai main part yei ho yesma kam gara 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveRight();
    }
    sprite.move(0, -200 * deltaTime); 
}

void Car::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Car::moveLeft() {
    if (lanePosition > 0) {
        lanePosition--;
        sprite.move(-200, 0); 
    }
}

void Car::moveRight() {
    if (lanePosition < 2) {
        lanePosition++;
        sprite.move(200, 0);
    }
}

sf::FloatRect Car::getBounds() const {
    return sprite.getGlobalBounds();
}

void Car::checkCollision(const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        if (sprite.getGlobalBounds().intersects(obstacle.getBounds())) {
            // gadi thokida hune kam 
            std::cout << "Collision with obstacle!" << std::endl;
           //game cancel garne
        }
    }
}

void Car::checkCollision(const std::vector<PowerUp>& powerUps) {
    for (const auto& powerUp : powerUps) {
        if (sprite.getGlobalBounds().intersects(powerUp.getBounds())) {
            // duita powerup lida ke garne 
            std::cout << "Collected power-up!" << std::endl;
            // powerup le ke garne  increse speed or obstacle stop 
        }
    }
}
