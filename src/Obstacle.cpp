#include "Obstacle.h"
#include <iostream>


Obstacle::Obstacle(sf::Texture& obstacleTexture): speedFactor{200.0f} {
   
    textureSize = obstacleTexture.getSize();
    obstacleSprite.setTexture(obstacleTexture);
    obstacleSprite.rotate(270);
    sf::Vector2f spriteSize(100.0f, 100.0f); 
    obstacleSprite.setScale(spriteSize.x / textureSize.x, spriteSize.y / textureSize.y);
    obstacleTexture.setSmooth(true); // Optional: enables smoothing
    obstacleTexture.setRepeated(false); // Ensure it's not being repeated
}

void Obstacle::update(float deltaTime)
{
    if (obstacleSprite.getPosition().y > 700)
    {
        isOffScreen = true;
    }
    obstacleSprite.move(0, speedFactor * deltaTime);
    speedFactor += 10 * deltaTime;
}

void Obstacle::render(sf::RenderWindow& window) const {

    //A red rectangle in the obstacle to show its bounds as collision was happening without touching the actual image
   sf::FloatRect carBounds = obstacleSprite.getGlobalBounds();
   sf::RectangleShape rect(obstacleSprite.getPosition());
   rect.setSize(sf::Vector2f(carBounds.width, carBounds.height));
   rect.setPosition(carBounds.left, carBounds.top);
   rect.setOutlineThickness(5.0f);
   rect.setOutlineColor(sf::Color::Red);
   rect.setFillColor(sf::Color::Transparent);
   window.draw(rect);
   
   window.draw(obstacleSprite);
}

sf::Vector2f Obstacle::getPosition() const
{
    return this->obstacleSprite.getPosition();
}

void Obstacle::setPosition(sf::Vector2f position)
{
    this->obstacleSprite.setPosition(position);
}

sf::FloatRect Obstacle::getBounds() const
{
    return obstacleSprite.getGlobalBounds();
}

bool Obstacle::ifOffScreen() const
{
    return isOffScreen;
}

