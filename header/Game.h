#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "Obstacle.h"
#include "PowerUp.h"
#include "Menu.h"
#include <vector>
#include"GameState.h" 
class Game {
private:
    sf::RenderWindow window;
    GameState currentState;
    Car playerCar;
    sf::Texture obstacleTexture;
    sf::Texture powerUpTexture;
    sf::Texture roadTexture;
    sf::Sprite roadSprite1;
    sf::Sprite roadSprite2;
    std::vector<Obstacle> activeObstacles;
    std::vector<PowerUp> activePowerUps;
    sf::Clock frame_clock;
    PowerUp powerUp;
    Menu menu;
    float speed;
    float obstacleSpawnTimer;
    float powerUpSpawnTimer;
    const float obstacleSpawnInterval = 2.0f;
    const float powerUpSpawnInterval = 10.0f;
    int score;


    void initWindow();
    void processEvents();
    void update(float deltaTime);
    void render();
    void spawnObstacles(float);
    void spawnPowerUps(float);
    void reset_game();
   

public:
    Game();
    void run();
};

