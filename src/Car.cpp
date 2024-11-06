#include "../header/Car.h"
#include "../header/GameState.h"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SOCKET;  // Define SOCKET as int on Linux
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

Car::Car(const std::string& textureFile) {
    std::string fullPath = "assets/" + textureFile;
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Error loading texture from " << fullPath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(400.f, 450.f);
    initializeSocket();
}

void Car::initializeSocket() {
    const char* serverIP = "192.168.1.42"; // Replace with your server IP
    int serverPort = 65432;

#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }
#endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/Address not supported." << std::endl;
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        close(sockfd);
#endif
        return;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed." << std::endl;
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        close(sockfd);
#endif
        return;
    }
}

void Car::update(float deltaTime) {
    if (isShieldActive) {
        shieldTime -= deltaTime;
        if (shieldTime <= 0.f) {
            isShieldActive = false;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        moveLeft(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        moveRight(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        moveUp(deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        moveDown(deltaTime);
    }

    char buffer[1024] = { 0 };
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string gesture(buffer);

        if (gesture.find("Pointing Left") != std::string::npos) {
            gmoveLeft();
        }
        else if (gesture.find("Pointing Right") != std::string::npos) {
            gmoveRight();
        }
         else if (gesture.find("Pointing Up") != std::string::npos) {
            gmoveUp();
        }
        else if (gesture.find("Pointing Down") != std::string::npos) {
            gmoveDown();
        }
    }
    else if (bytesReceived == 0) {
        std::cerr << "Connection closed by the server." << std::endl;
    }
}

void Car::render(sf::RenderWindow& window) {
    if (isShieldActive) {
        sf::CircleShape shieldCircle(50.f);
        shieldCircle.setFillColor(sf::Color(0, 255, 0, 128));
        shieldCircle.setPosition(sprite.getPosition().x - 25.f, sprite.getPosition().y - 25.f);
        window.draw(shieldCircle);
    }
    window.draw(sprite);
}

void Car::activateShield(float time) {
    isShieldActive = true;
    shieldTime = time;
}

void Car::moveLeft(float deltaTime) {
    if (sprite.getPosition().x - 1 >= 0) {
        sprite.move(-speed * deltaTime, 0);
    }
}

void Car::moveRight(float deltaTime) {
    if (sprite.getPosition().x + sprite.getGlobalBounds().width < 800) {
        sprite.move(speed * deltaTime, 0);
    }
}

void Car::moveUp(float deltaTime) {
    if (sprite.getPosition().y - 1 >= 0) {
        sprite.move(0, -speed * deltaTime);
    }
}

void Car::moveDown(float deltaTime) {
    if (sprite.getPosition().y + sprite.getGlobalBounds().height < 530) {
        sprite.move(0, speed * deltaTime);
    }
}

sf::FloatRect Car::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Car::checkCollision(const std::vector<Obstacle>& obstacles) {
    if (isShieldActive) {
        return false;
    }
    for (const auto& obstacle : obstacles) {
        if (sprite.getGlobalBounds().intersects(obstacle.getBounds())) {
            std::cout << "Collision with obstacle!" << std::endl;
            return true;
        }
    }
    return false;
}

bool Car::checkCollision(std::vector<PowerUp>& powerUps) {
    for (auto it = powerUps.begin(); it != powerUps.end(); ) {
        if (sprite.getGlobalBounds().intersects(it->getBounds())) {
            std::cout << "Collected power-up!" << std::endl;
            activateShield(6.9f);
            it = powerUps.erase(it);
            return true;
        }
        else {
            ++it;
        }
    }
    return false;
}

Car::~Car() {
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
}

void Car::gmoveRight() {
    if (sprite.getPosition().x < 750) {
        sprite.move(4.0f, 0);
    }
}

void Car::gmoveLeft() {
    if (sprite.getPosition().x > 0) {
        sprite.move(-4.0f, 0);
    }
}

void Car::gmoveUp() {
    if (sprite.getPosition().y > 0) {
        sprite.move(0, -4.0f);
    }
}

void Car::gmoveDown() {
    if (sprite.getPosition().y + sprite.getGlobalBounds().height < 530) {
        sprite.move(0, 4.0f);
    }
}

