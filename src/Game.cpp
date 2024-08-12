#include "Game.h"
#include "GameState.h" 
#include "Obstacle.h"
#include <iostream>


Game::Game() : playerCar("player_car.png"), speed(100.0f), obstacleSpawnTimer(0.0f), powerUpSpawnTimer(0.0f), obstacleSpawnInterval(3.0f), powerUpSpawnInterval(5.0f) {
    this->initWindow();
    if (!obstacleTexture.loadFromFile("assets/obstacle.png"))
    {
        std::cerr << "Failed to load obstacle texture" << std::endl;
    }
    if (!powerUpTexture.loadFromFile("assets/powerup.png"))
    {
        std::cerr << "Failed to load power up texture" << std::endl;
    }
    if (!roadTexture.loadFromFile("assets/road.png"))
    {
        std::cerr << "Failed to load road texture" << std::endl;
    }
    roadSprite1.setTexture(roadTexture);
    sf::Vector2u textureSize = roadTexture.getSize();
    roadSprite1.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    roadSprite1.rotate(90);
    roadSprite1.setPosition(400.f, 300.f);
    roadSprite1.setScale(600.f / textureSize.x, 800.f / textureSize.y);

    roadSprite2.setTexture(roadTexture);
    roadSprite2.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    roadSprite2.rotate(90);
    roadSprite2.setPosition(400.f, 300.f - 600.f);
    roadSprite2.setScale(600.f / textureSize.x, 800.f / textureSize.y);

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
    roadSprite1.move(0, 100 * deltaTime);
    roadSprite2.move(0, 100 * deltaTime);
    speed += 0.05f * deltaTime;

    obstacleSpawnTimer += deltaTime;
    powerUpSpawnTimer += deltaTime;

    spawnObstacles(deltaTime);
    spawnPowerUps(deltaTime);

    for (auto& obstacle : activeObstacles)
    {
        obstacle.update(deltaTime);
    }
    for (auto& powerUp : activePowerUps)
    {
        powerUp.update(deltaTime);
    }
    

    if (playerCar.checkCollision(activeObstacles))
    {
        this->window.clear();
        currentState = GameState::GAME_OVER;
    }
    playerCar.checkCollision(activePowerUps);
}

void Game::render() {

    this->window.draw(roadSprite1);
    this->window.draw(roadSprite2);
    playerCar.render(this->window);

    if (roadSprite1.getPosition().y >= 900.f) {
        roadSprite1.setPosition(roadSprite2.getPosition().x, roadSprite2.getPosition().y - 600.f);
    }

    if (roadSprite2.getPosition().y >= 900.f) {
        roadSprite2.setPosition(roadSprite1.getPosition().x, roadSprite1.getPosition().y - 600.f);
    }

    for (auto& obstacle : activeObstacles)
    {

        obstacle.render(this->window);
    }

    for (auto& powerUp : activePowerUps)
    {
        powerUp.render(this->window);
    }
    
}
void Game::spawnObstacles(float deltaTime) {
    
    if (obstacleSpawnTimer >= obstacleSpawnInterval) {
        Obstacle obstacle(obstacleTexture);
        obstacleSpawnTimer = 0.0f;
        activeObstacles.push_back(obstacle);
    }
}

void Game::spawnPowerUps(float elapsedTime) {

   
    if (powerUpSpawnTimer >= powerUpSpawnInterval) {
        powerUpSpawnTimer = 0.0f;
        PowerUp powerup(powerUpTexture);
        activePowerUps.push_back(powerup);
    }
}

void Game::reset_game()
{
    playerCar.sprite.setPosition(400.f , 500.f);
    activeObstacles.clear();
    activePowerUps.clear();
    
    obstacleSpawnTimer = 0.0f;
    powerUpSpawnTimer = 0.0f;

    frame_clock.restart();
    score = 0;
    
}


void Game::run() {
    
    sf::Clock game_clock;
    currentState = GameState::MENU;
    while (this->window.isOpen())
    {
        float deltaTime = frame_clock.restart().asSeconds();
        this->window.clear();
        this->processEvents();

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
                    currentState = GameState::HIGH_SCORE;
                }
                else if (selected == 4)
                {
                    this->window.close();
                }
            }
            break;
        }
        case GameState::PLAYING:
        {
            // Code/Functions for What to do when playing
            this->update(deltaTime);
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
            std::cout << "Game over!";
            this->reset_game();
            currentState = GameState::MENU;
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

