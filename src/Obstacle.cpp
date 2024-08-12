#include "Obstacle.h"
#include <iostream>


Obstacle::Obstacle(sf::Texture& obstacleTexture) {
   
    textureSize = obstacleTexture.getSize();
    std::cout << textureSize.x;
    obstacleSprite.setTexture(obstacleTexture);
    obstacleSprite.rotate(270);
    float x_position = static_cast<float>(rand() % (800 - 100)); // Random x within window width
    float y_position = 300.f; 
    obstacleSprite.setPosition(x_position , y_position);
    sf::Vector2f spriteSize(100.0f, 100.0f); 
    obstacleSprite.setScale(spriteSize.x / textureSize.x, spriteSize.y / textureSize.y);
    obstacleTexture.setSmooth(true); // Optional: enables smoothing
    obstacleTexture.setRepeated(false); // Ensure it's not being repeated
}

void Obstacle::update(float deltaTime)
{
    if (obstacleSprite.getPosition().y > 600)
    {
        isOffScreen = true;
    }
    obstacleSprite.move(0, 100 * deltaTime);
}

void Obstacle::render(sf::RenderWindow& window) const {
 
    window.draw(obstacleSprite);
}

sf::FloatRect Obstacle::getBounds() const
{
    return obstacleSprite.getGlobalBounds();
}



bool Obstacle::ifOffScreen() const
{
    return isOffScreen;
}

