#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Obstacle.h"
#include "PowerUp.h"

#ifdef _WIN32
    #include <winsock2.h>  // Windows socket programming
    #include <ws2tcpip.h>  // For `inet_pton`
    #pragma comment(lib, "ws2_32.lib")  // Link with Winsock library
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>    // For close() on Linux
    typedef int SOCKET;    // Define SOCKET as int on Linux
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

class Car {
private:
    sf::Sprite sprite;
    sf::Texture texture;

    std::vector<sf::Texture> obstacleTextures;  

    // Socket-related members
    SOCKET sockfd;             // Socket file descriptor
    sockaddr_in serv_addr;      // Server address structure

    void initializeSocket();    // Function to initialize socket connection
    void loadObstacleTextures(); // Function to load multiple obstacle textures
    sf::Texture getRandomObstacleTexture(); // Function to get a random obstacle texture

public:
    Car(const std::string& textureFile);  // Constructor
    ~Car();  // Destructor to close the socket

    bool isShieldActive = false;
    float shieldTime = 0.0f;
    float speed = 200.f;
    int lanePosition = 1;

    void update(float deltaTime);
    void render(sf::RenderWindow& window);

    void moveLeft(float deltaTime);   // Updated with deltaTime
    void moveRight(float deltaTime);  // Updated with deltaTime
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void gmoveLeft();
    void gmoveRight();
    void gmoveUp();
    void gmoveDown();
    
    void activateShield(float time);  // Added declaration for activateShield
    sf::FloatRect getBounds() const;
    
    bool checkCollision(const std::vector<Obstacle>& obstacles);
    bool checkCollision(std::vector<PowerUp>& powerUps);

    friend class Game;
};
