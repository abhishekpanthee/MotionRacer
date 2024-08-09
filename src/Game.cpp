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
        sf::Clock frame_clock;
    sf::Clock game_clock;
    GameState currentState = GameState::MENU;
    while (this->window.isOpen())
    {
        float deltaTime = frame_clock.restart().asSeconds();
        
        this->processEvents();
        this->window.clear();
        switch (currentState)
        {
        case GameState::MENU:
        {
            bool enterPressed = menu.updateMenu(this->window);
            menu.drawMenu(this->window);
            if (enterPressed)
            {

                int selected = menu.getClickedItem();
                if (selected == 0)
                {
                    currentState = GameState::PLAYING;
                }
                else if (selected == 1)
                {
                    currentState = GameState::HOW_TO_PLAY;
                }
                else if (selected == 2)
                {
                    currentState = GameState::SETTINGS;
                }
                else if (selected == 3)
                {
                    this->window.close();
                }
            }
            break;
        }
        case GameState::PLAYING:
        {
            // Code/Functions for What to do when playing

            //Time elapsed after the game started
            float elapsedTime = game_clock.getElapsedTime().asSeconds();
            
            // Still Working on the implementation
            this->update(deltaTime, elapsedTime);
            this->render();
            break;
        }
        case GameState::PAUSED:
        {
            // Code/Functions for Paused Condition
            break;
        }
        case GameState::GAME_OVER:
        {
            // Code/Functions For Game over
            break;
        }
        case GameState::SETTINGS:
        {
            // Code/Functions for settings
            break;
        }
        case GameState::HOW_TO_PLAY:
        {
            // Code/Functions for how to play
            break;
        }
        }
           
        this->window.display();
       
    }
}
