#include "Game.h"
#include <iostream>

Game::Game() : playerCar("player_car.png"), speed(100.0f), obstacleSpawnTimer(0.0f), powerUpSpawnTimer(0.0f) {
    this->initWindow();
}

void Game::initWindow() {
    this->window.create(sf::VideoMode(800, 600), "Car Racing Game");
}

void Game::processEvents() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        }
    }
}

void Game::update(float deltaTime) {
    playerCar.update(deltaTime);
    speed += 0.1f * deltaTime;

    obstacleSpawnTimer += deltaTime;
    powerUpSpawnTimer += deltaTime;

    spawnObstacles(deltaTime);
    spawnPowerUps(deltaTime);

    playerCar.checkCollision(obstacles);
    playerCar.checkCollision(powerUps);
}

void Game::render() {
    playerCar.render(this->window);

    for (const auto& obstacle : obstacles) {
        obstacle.render(this->window);
    }

    for (const auto& powerUp : powerUps) {
        powerUp.render(this->window);
    }
}

void Game::spawnObstacles(float deltaTime) {
    if (obstacleSpawnTimer >= obstacleSpawnInterval) {
        obstacles.push_back(Obstacle("obstacle.png", rand() % 3 * 200, -100)); 
        obstacleSpawnTimer = 0.0f;
    }
}

void Game::spawnPowerUps(float deltaTime) {
    if (powerUpSpawnTimer >= powerUpSpawnInterval) {
        powerUps.push_back(PowerUp("powerup.png", rand() % 3 * 200, -100)); 
        powerUpSpawnTimer = 0.0f;
    }
}

void Game::run() {
    sf::Clock clock;

while (this->window.isOpen())
{
    float deltaTime = clock.restart().asSeconds();
    this->processEvents();
    this->window.clear();
    if (menu.isActive())
    {
        std::cout << "Menu Active";
        bool enterPressed = menu.updateMenu(this->window);
        menu.drawMenu(this->window);
        if (enterPressed)
        {

            int selected = menu.getClickedItem();
            if (selected == 0)
            {
                menu.deactivate();
                std::cout << "Menu deactivated";
            }
            else if (selected == 3)
            {
                this->window.close();
            }
        }
    }
        else
        {
            this->update(deltaTime);
            this->render();
        }
    this->window.display();
   
}
}
