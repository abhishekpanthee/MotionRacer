#include "../header/PowerUp.h"
#include <iostream>

PowerUp::PowerUp(){}
PowerUp::PowerUp(sf::Texture& powerUpTexture):isOffScreen{false} {
    //  image ko directory texture file ma laijane path contrsut gareko 

    sprite.setTexture(powerUpTexture);
    textureSize = powerUpTexture.getSize();
    std::cout << textureSize.x;
    sprite.setTexture(powerUpTexture);
    sprite.rotate(270);
    float x_position = static_cast<float>(rand() % (800 - 100)); // Random x within window width
    float y_position = 300.f; 
    sprite.setPosition(x_position, y_position);
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
    if (sprite.getPosition().y > 600)
    {
        return true;  // Indicate that the PowerUp should be deleted
    }

    sprite.move(0, 100 * deltaTime);
    return false;  // Indicate that the PowerUp should not be deleted
}


sf::FloatRect PowerUp::getBounds() const {
    // tyo sprite le gareko lai bounded garako 
    return sprite.getGlobalBounds();
}
