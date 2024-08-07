#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Obstacle.h"
#include "PowerUp.h"
#include <vector>

class Game {
private:
    sf::RenderWindow window;
    Car playerCar;
    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerUps;
    float speed;
    float obstacleSpawnTimer;
    float powerUpSpawnTimer;
    const float obstacleSpawnInterval = 2.0f;
    const float powerUpSpawnInterval = 10.0f; 
    void initWindow();
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnObstacles(float deltaTime);
    void spawnPowerUps(float deltaTime);

public:
    Game();
    void run();
};
