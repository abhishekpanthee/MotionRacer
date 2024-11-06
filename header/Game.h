#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
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
    GameState previousState;
    float speedIncreaseTimer;
    Car playerCar;
    sf::Font font;
    // Texts for the various states.
    sf::Text gameOverText;
    sf::Text howToPLaymain;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text titleText;
    sf::Font yourFont;
    sf:: Music drivingMusic;
    sf::Font emojiFont;
    
    sf::Texture obstacleTexture1;
    sf::Texture obstacleTexture2;
    sf::Texture obstacleTexture3;

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
    float speedFactor;
    float obstacleSpawnTimer;
    float powerUpSpawnTimer;
    const float obstacleSpawnInterval;
    const float powerUpSpawnInterval;
    const float minDistance;
    int score;
    int highScore;
    float scoreTimer;


    void initWindow();
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void spawnObstacles(float);
    void spawnPowerUps(float);
    void reset_game();
    void displayResumeButton();          
    bool isResumeButtonClicked(); 
    
    bool usernameEntered;
    
 void loadHighScore(); // Load high score from a file
 void updateScoreDisplay(); // Update the display of the score and high score
 void resetHighScore(); // New method to reset high score to zero
    float distance(sf::Vector2f pos1, sf::Vector2f pos2);
   

public:
 std::string getUsername();
    void saveHighScore(const std::string& username);
    Game();
    static std::string username;
    bool howToPlayClicked;
     void closeWindow();
     void initializeGameWindow();
      void displayLeaderboard(GameState &currentState);
    void run();
    void start();
    void initTextures();
    std::string generateRandomUsername();
    sf::Texture* getRandomObstacleTexture();
};

