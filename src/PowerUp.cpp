#include "PowerUp.h"
#include <iostream>

PowerUp::PowerUp(const std::string& textureFile, float x, float y) {
    //  image ko directory texture file ma laijane path contrsut gareko 
    std::string fullPath = "assets/" + textureFile; 

    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Failed to load image " << fullPath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
}

void PowerUp::render(sf::RenderWindow& window) const {
    
    window.draw(sprite);
}

sf::FloatRect PowerUp::getBounds() const {
    // tyo sprite le gareko lai bounded garako 
    return sprite.getGlobalBounds();
}