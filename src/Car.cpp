#include "Car.h"
#include "GameState.h"
#include <SFML/Window.hpp>
#include <iostream>

Car::Car(const std::string& textureFile) : lanePosition(1) {
    // image ko directory texture file ma laijane 
    std::string fullPath = "assets/" + textureFile;
    //eroor handling
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Error loading texture from " << fullPath << std::endl;
    }
    sf::Vector2u textureSize = texture.getSize();
    sprite.setTexture(texture);
    sprite.setPosition(400.f, 500.f);
    sprite.rotate(90);
    sf::Vector2f spriteSize(100.0f, 100.0f); // Assuming your sprite size should be 50x50
    sprite.setScale(spriteSize.x / textureSize.x, spriteSize.y / textureSize.y);
}

void Car::update(float deltaTime) {
    // Rhythem, lai main part yei ho yesma kam gara 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveRight();
    }

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

bool Car::checkCollision(const std::vector<Obstacle>& obstacles) {
    for (const auto& obstacle : obstacles) {
        if (sprite.getGlobalBounds().intersects(obstacle.getBounds())) {
            // gadi thokida hune kam 
            std::cout << "Collision with obstacle!" << std::endl;
            return true;
            //game cancel garne
        }
    }
    return false;
}

bool Car::checkCollision(const std::vector<PowerUp>& powerUps) {
    for (const auto& powerUp : powerUps) {
        if (sprite.getGlobalBounds().intersects(powerUp.getBounds())) {
            // duita powerup lida ke garne 
            std::cout << "Collected power-up!" << std::endl;
            return true;
            // powerup le ke garne  increse speed or obstacle stop 
        }
    }
    return false;
}
