#include "../header/Car.h"
#include "../header/GameState.h"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <winsock2.h>    // For Windows socket functions
#include <ws2tcpip.h>    // For `inet_pton` in Windows

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
    initializeSocket();
    
}

void Car::update(float deltaTime) {
    // Check keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveRight();
    }

    // Check gesture input from Python server
    char buffer[1024] = { 0 };
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::string gesture(buffer);

        if (gesture.find("Pointing Left") != std::string::npos) {
            gmoveLeft();
        }
        else if (gesture.find("Pointing Right") != std::string::npos) {
            gmoveRight();
        }
        else if (gesture.find("five") != std::string::npos) {
            //accelarate wala function haleseyo
        }
        else if (gesture.find("fist") != std::string::npos) {
            //stop wala function haleseyo
            
        }
      
       
    }
    else if (bytesReceived == 0) {
        // Connection closed
        std::cerr << "Connection closed by the server." << std::endl;
        // Handle disconnection if needed
    }
    else {
        // Error occurred
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }
}



void Car::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Car::moveLeft() {
  
    if (sprite.getPosition().x - 1 >= 0) {
        sprite.move(-4.0f, 0);
    }

   

}

void Car::moveRight() {
 
    if (sprite.getPosition().x  < 750) {
        sprite.move(4.0f, 0);
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



bool Car::checkCollision(std::vector<PowerUp>& powerUps) {
    for (auto it = powerUps.begin(); it != powerUps.end(); ) {
        if (sprite.getGlobalBounds().intersects(it->getBounds())) {
            std::cout << "Collected power-up!" << std::endl;

            // Remove the power-up from the vector and update the iterator
            it = powerUps.erase(it);
        }
        else {
            ++it; // Only increment the iterator if no element was erased
        }
    }
    return false; // Return true if collision is detected, or other logic if needed
}

void Car::initializeSocket() {
    const char* serverIP = "192.168.1.8"; // Replace with your server IP
    int serverPort = 65432;

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }
}

Car::~Car() {
    // Close the socket connection
    closesocket(sockfd);
    WSACleanup();
}

void Car::gmoveRight() {
   
    if (sprite.getPosition().x < 750) {
        sprite.move(4.0f, 0);
    }
}
void Car::gmoveLeft() {
    
    if (sprite.getPosition().x < 750) {
        sprite.move(-4.0f, 0);
    }
}

