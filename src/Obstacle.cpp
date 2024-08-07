#include "Obstacle.h"
#include <iostream>

Obstacle::Obstacle(const std::string& textureFile, float x, float y) {
    /// image ko directory texture file ma laijane  yo chai assocoate image chai .h file ma specify garexha
    std::string fullPath = "assets/" + textureFile; 
    
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Failed to load image " << fullPath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Obstacle::getBounds() const {
   // tyo spr le gareko lai bounded garako 
    return sprite.getGlobalBounds();
}
