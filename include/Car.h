#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Obstacle.h"
#include "PowerUp.h"
#include <winsock2.h>  // For Windows socket programming
#include <ws2tcpip.h>  // For `inet_pton`

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

class Car {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float lanePosition;

    // Socket-related members
    SOCKET sockfd;             // Socket file descriptor (Windows-specific)
    sockaddr_in serv_addr;     // Server address structure

    void initializeSocket();   // Function to initialize socket connection

public:
    Car(const std::string& textureFile);
    ~Car();                    // Destructor to close the socket
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void moveLeft();
    void moveRight();
    void gmoveLeft();
    void gmoveRight();
    sf::FloatRect getBounds() const;
    bool checkCollision(const std::vector<Obstacle>& obstacles);
    bool checkCollision(std::vector<PowerUp>& powerUps);

    friend class Game;
};

