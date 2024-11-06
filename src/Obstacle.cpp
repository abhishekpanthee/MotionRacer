#include "../header/Obstacle.h"
#include <iostream>

// Constructor: Initializes the obstacle sprite with the given texture
Obstacle::Obstacle(sf::Texture& obstacleTexture): speedFactor{200.0f} {
    // Get the texture size and set it to the sprite
    textureSize = obstacleTexture.getSize();
    obstacleSprite.setTexture(obstacleTexture);

    // Rotate the obstacle sprite by 270 degrees
    obstacleSprite.rotate(270);

    // Set the sprite size (scale it down to a desired size)
    sf::Vector2f spriteSize(100.0f, 100.0f);  // Desired size for obstacle
    obstacleSprite.setScale(spriteSize.x / textureSize.x, spriteSize.y / textureSize.y);

    // Optionally set texture properties (smooth or not, repeating behavior)
    obstacleTexture.setSmooth(true);  // Smooth textures
    obstacleTexture.setRepeated(false);  // No repeating
}

// Update the position and speed of the obstacle
void Obstacle::update(float deltaTime) {
    // If the obstacle has passed beyond the screen, mark it as offscreen
    if (obstacleSprite.getPosition().y > 700) {
        isOffScreen = true;
    }

    // Move the obstacle downwards (speed increases over time)
    obstacleSprite.move(0, speedFactor * deltaTime);

    // Increase the speed of the obstacle over time
    speedFactor += 10 * deltaTime;
}

// Render the obstacle on the given window
void Obstacle::render(sf::RenderWindow& window) const {
    // For debugging, draw a red rectangle around the obstacle's bounds
    sf::FloatRect carBounds = obstacleSprite.getGlobalBounds();
    sf::RectangleShape rect(obstacleSprite.getPosition());
    rect.setSize(sf::Vector2f(carBounds.width, carBounds.height));
    rect.setPosition(carBounds.left, carBounds.top);
    rect.setOutlineThickness(5.0f);
    rect.setOutlineColor(sf::Color::Transparent);
    rect.setFillColor(sf::Color::Transparent);
    window.draw(rect);

    // Draw the obstacle sprite
    window.draw(obstacleSprite);
}

// Get the position of the obstacle sprite
sf::Vector2f Obstacle::getPosition() const {
    return this->obstacleSprite.getPosition();
}

// Set a new position for the obstacle sprite
void Obstacle::setPosition(sf::Vector2f position) {
    this->obstacleSprite.setPosition(position);
}

// Get the bounding box of the obstacle sprite
sf::FloatRect Obstacle::getBounds() const {
    return obstacleSprite.getGlobalBounds();
}

// Check if the obstacle is off the screen
bool Obstacle::ifOffScreen() const {
    return isOffScreen;
}
