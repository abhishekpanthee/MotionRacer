#include "../header/PowerUp.h"
#include <iostream>

PowerUp::PowerUp() : speedFactor {200.0f} {}
PowerUp::PowerUp(sf::Texture& powerUpTexture): speedFactor {200.0f} {
    
    textureSize = powerUpTexture.getSize();
    sprite.setTexture(powerUpTexture);
    sprite.rotate(270);
    sf::Vector2f spriteSize(50.0f, 50.0f); 
    sprite.setScale(spriteSize.x / textureSize.x, spriteSize.y / textureSize.y);
    powerUpTexture.setSmooth(true); // Optional: enables smoothing
    powerUpTexture.setRepeated(false); // Ensure it's not being repeated    
}

void PowerUp::render(sf::RenderWindow& window) const {
    
    window.draw(sprite);
}


bool PowerUp::update(float deltaTime)
{
    if (sprite.getPosition().y > 700)
    {
        return true;  // Indicate that the PowerUp should be deleted
    }

    sprite.move(0 , speedFactor * deltaTime);
    return false;  // Indicate that the PowerUp should not be deleted
}

sf::Vector2f PowerUp::getPosition() const
{
    return this->sprite.getPosition();
}

void PowerUp::setPosition(sf::Vector2f position)
{
    this->sprite.setPosition(position);
}

sf::FloatRect PowerUp::getBounds() const {
    // tyo sprite le gareko lai bounded garako 
    return sprite.getGlobalBounds();
}
