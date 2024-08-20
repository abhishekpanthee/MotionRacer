#include "../header/Game.h"
#include "../header/GameState.h" 
#include "../header/Obstacle.h"
#include <iostream>


Game::Game() : playerCar("player_car.png"), speedFactor(100.0f), obstacleSpawnTimer(0.0f), powerUpSpawnTimer(0.0f), obstacleSpawnInterval(3.0f), powerUpSpawnInterval(10.0f) , score{0}, highScore {0} , minDistance{100.0f} {
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

    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER :(");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(100.f, 100.f);
    
    howToPLay.setFont(font);
    howToPLay.setString("Use the Arrow keys to move the car. Avoid obstacles, \n take powerups and try to survive as long as you can");
    howToPLay.setCharacterSize(24);
    howToPLay.setFillColor(sf::Color::White);
    howToPLay.setPosition(50.f, 100.f);
    
    scoreText.setFont(font);
    std::string score_str = std::to_string(score);
    scoreText.setString(score_str);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(700.f, 20.f);

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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        currentState = GameState::PAUSED;
    }
    }
}



void Game::update(float deltaTime) {
 
    playerCar.update(deltaTime);
    roadSprite1.move(0, 100 * deltaTime);
    roadSprite2.move(0, 100 * deltaTime);
    
    speedFactor += 10 * deltaTime;
    std::string score_str = std::to_string(score);
    scoreText.setString(score_str);

    obstacleSpawnTimer += 3 * deltaTime;
    powerUpSpawnTimer += 0.5 * deltaTime;

    spawnObstacles(deltaTime);
    spawnPowerUps(deltaTime);

    for (auto it = activeObstacles.begin(); it != activeObstacles.end();)
    {
    it->update(deltaTime);
    if (it->ifOffScreen())
    {
        it = activeObstacles.erase(it);
        score++;
    }
    else
    {
        ++it;
    }
    }
    for (auto it = activePowerUps.begin(); it != activePowerUps.end(); ) {
        if (it->update(deltaTime)) {
            it = activePowerUps.erase(it);  // Delete the power-up if it went off-screen
        }
        else {
            ++it;  // Move to the next power-up
        }
    }

    if (playerCar.checkCollision(activeObstacles))
    {
        this->window.clear();
         if (score >= highScore)
         {
         highScore = score;
         }
        currentState = GameState::GAME_OVER;
    }
    if(playerCar.checkCollision(activePowerUps))
    {
        score += 2;
    }

}


void Game::render() {

    this->window.draw(roadSprite1);
    this->window.draw(roadSprite2);
    this->window.draw(scoreText);
    
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
    bool validSpawn = true;
    sf::Vector2f newPosition(rand() % 700, -50);  // Random X position, Y just above the screen

    // Check against existing obstacles
    for (const auto& obstacle : activeObstacles) {
        if (distance(obstacle.getPosition(), newPosition) < minDistance) {
            validSpawn = false;
            break;
        }
    }

    // Check against existing power-ups
    for (const auto& powerUp : activePowerUps) {
        if (distance(powerUp.getPosition(), newPosition) < minDistance) {
            validSpawn = false;
            break;
        }
    }

    if (validSpawn) {
        Obstacle obstacle(obstacleTexture);
        obstacle.setPosition(newPosition);
        activeObstacles.push_back(obstacle);
        obstacleSpawnTimer = 0.0f;
    }
}
}

void Game::spawnPowerUps(float elapsedTime) {
    
if (!playerCar.isShieldActive && powerUpSpawnTimer >= powerUpSpawnInterval) {
    bool validSpawn = true;
    sf::Vector2f newPosition(rand() % 700, -50);  // Random X position, Y just above the screen

    // Check against existing power-ups
    for (const auto& powerUp : activePowerUps) {
        if (distance(powerUp.getPosition(), newPosition) < minDistance) {
            validSpawn = false;
            break;
        }
    }

    for (const auto& obstacle : activeObstacles) {
        if (distance(obstacle.getPosition(), newPosition) < minDistance) {
            validSpawn = false;
            break;
        }
    }
    if (validSpawn) {
        PowerUp powerup(powerUpTexture);
        powerup.setPosition(newPosition);
        activePowerUps.push_back(powerup);
        powerUpSpawnTimer = 0.0f;
    }
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
    speedFactor = 100.0f;
}
float Game::distance(sf::Vector2f pos1, sf::Vector2f pos2)
{
    return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
}

void Game::run() {
    sf::Clock game_clock;
currentState = GameState::MENU;
Menu settings(1);
sf::Event event;
bool keyPressed = false;
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
        
       
        sf::sleep(sf::milliseconds(200));
        currentState = GameState::MENU;
        break;
    }
    case GameState::GAME_OVER:
    {
        // Code/Functions For Game over
        std::cout << "Game over!";
        std::cout << "Score: " << score;
        this->window.clear();
        this->window.draw(gameOverText); 
        this->window.display();
        this->reset_game();
        while (!keyPressed)
        {
            while (this->window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                {
                    keyPressed = true;
                    break;
                }
                else if (event.type == sf::Event::Closed)
                {
                    this->window.close();
                    keyPressed = true;  // Break loop if window is closed
                }
            }
        }
        sf::sleep(sf::milliseconds(200));
        currentState = GameState::MENU;
        break;
    }
    case GameState::SETTINGS:
    {
        // Code/Functions for settings
        bool enterPressed = settings.updateMenu(this->window);
        settings.drawMenu(this->window);
        //currentState = GameState::MENU;
        break;
    }
    case GameState::HOW_TO_PLAY:
    {
        // Code/Functions for how to play
       
        this->window.clear();
        this->window.draw(howToPLay);

        while (!keyPressed)
        {
            while (this->window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                {
                    keyPressed = true;
                    currentState = GameState::MENU;
                    break;
                }
                else if (event.type == sf::Event::Closed)
                {
                    this->window.close();
                    keyPressed = true;  // Break loop if window is closed
                }
            }
        }
        
        break;
    }

    case GameState::HIGH_SCORE:
    {
        highScoreText.setFont(font);
        std::string highScoreStr = std::to_string(highScore);
        highScoreText.setString(highScoreStr);
        highScoreText.setCharacterSize(48);
        highScoreText.setFillColor(sf::Color::White);
        highScoreText.setPosition(350.f, 400.f);
        this->window.draw(highScoreText);

        while (!keyPressed)
        {
            while (this->window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                {
                    keyPressed = true;
                    currentState = GameState::MENU;
                    break;
                }
                else if (event.type == sf::Event::Closed)
                {
                    this->window.close();
                    keyPressed = true;  // Break loop if window is closed
                }
            }
        }
        
    }
    }
       
    this->window.display();
   
}
}


