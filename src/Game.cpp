#include "../header/Game.h"
#include "../header/GameState.h"
#include "../header/Obstacle.h"
#include <iostream>
#include <fstream>  // Include for std::ifstream and std::ofstream
#include <cmath>    // Include for sqrt and pow functions
#include <SFML/Audio.hpp> 
#include <string>
#include <unordered_map>
std::string Game::username = "";
GameState currentState = GameState::MENU;
#include <random>
#include <ctime>

void Game::initializeGameWindow() {
    this->initWindow();
}

void Game::closeWindow() {
    this->window.close();
}



std::string Game::getUsername() {
    std::string username;
    sf::Text promptText;
    promptText.setFont(font);
    promptText.setString("Enter your username:");
    promptText.setCharacterSize(24);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(200, 200);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(200, 240);

    sf::Event event;
    while (this->window.isOpen()) {
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->closeWindow(); // Use closeWindow instead of directly calling window.close()

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && !username.empty()) { // Backspace
                    username.pop_back();
                } else if (event.text.unicode == 13) { // Enter key
                    this->usernameEntered = true; // Set flag to true
                    this->window.clear();
                    // If no username was entered, generate a random username
                    if (username.empty()) {
                        username = generateRandomUsername();
                    }
                    return username;
                }
                else if (username.size() < 15 && event.text.unicode < 128) { // ASCII limit
                    username += static_cast<char>(event.text.unicode);
                }
            }
        }

        inputText.setString(username);
        this->window.clear();
        this->window.draw(promptText);
        this->window.draw(inputText);
        this->window.display();
    }
    
    return username; // In case window closes, return current input
}

// Helper function to generate a random username of 8 lowercase letters
std::string Game::generateRandomUsername() {
    const std::string chars = "abcdefghijklmnopqrstuvwxyz";
    std::string randomUsername;
    std::random_device rd;
    std::mt19937 gen(rd());  // Initialize random number generator
    std::uniform_int_distribution<> dis(0, chars.size() - 1); // Random index

    for (int i = 0; i < 8; ++i) {
        randomUsername += chars[dis(gen)];
    }

    return randomUsername;
}





void Game::displayResumeButton() {
    sf::Text resumeText;
    resumeText.setFont(font);            // Assume you've loaded a font
    resumeText.setString("Resume");
    resumeText.setCharacterSize(24);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(300, 30);     // Set position on the screen

    // Create a rectangle to enclose the "Resume" text
    sf::FloatRect textBounds = resumeText.getLocalBounds();  // Get the bounds of the text
    sf::RectangleShape buttonBackground(sf::Vector2f(textBounds.width + 20.f, textBounds.height + 16.f)); // Add padding around text
    buttonBackground.setFillColor(sf::Color::Red);           // Set the rectangle color to red
    buttonBackground.setPosition(resumeText.getPosition().x - 10.f, resumeText.getPosition().y - 2.f); // Adjust position for padding

    // Get mouse position relative to the window
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::IntRect buttonArea(resumeText.getPosition().x - 10.f, resumeText.getPosition().y - 5.f, buttonBackground.getSize().x, buttonBackground.getSize().y);

    // Check if the mouse is inside the button area
    if (buttonArea.contains(mousePos)) {
        // Change text color to yellow if hovered over
        resumeText.setFillColor(sf::Color::Yellow);

        // Check for mouse click (left button)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentState = GameState::PLAYING;  // Change the game state to "PLAYING"
        }
    } else {
        // If not hovering, reset the text color
        resumeText.setFillColor(sf::Color::White);
    }

    // Draw the rectangle first, then draw the text on top of it
    window.draw(buttonBackground);
    window.draw(resumeText);  // Draw the text onto the game window
}



bool Game::isResumeButtonClicked() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get mouse position relative to window
    sf::IntRect resumeButtonArea(200, 200, 100, 50);       // Define the button area (adjust as needed)

    // Return true if mouse is within the button area and left button is clicked
    return resumeButtonArea.contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Game::displayLeaderboard(GameState &currentState) {
    std::ifstream file("../assets/highscore.dat");
    if (!file.is_open()) {
        std::cerr << "Could not open highscore.dat" << std::endl;
        currentState = GameState::MENU;
        return;
    }

    // Read scores and retain only the highest score for each username
    std::unordered_map<std::string, int> userScores;
    std::string username;
    int score;
    while (file >> username >> score) {
        if (userScores.find(username) == userScores.end() || userScores[username] < score) {
            userScores[username] = score;
        }
    }
    file.close();

    // Convert to vector and sort
    std::vector<std::pair<std::string, int>> scores(userScores.begin(), userScores.end());
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    this->window.clear();
    sf::Text textElement;
    textElement.setFont(font);

    // Leaderboard title
    textElement.setString("LEADERBOARD");
    textElement.setCharacterSize(36);
    textElement.setFillColor(sf::Color::White);
    float startX = this->window.getSize().x / 2.0f - textElement.getLocalBounds().width / 2;
    float startY = 80.0f;
    textElement.setPosition(startX, startY);
    this->window.draw(textElement);

    // Column headers
    float columnSpacing = 200.0f;
    startY += 80;
    textElement.setCharacterSize(28);
    textElement.setString("Position");
    textElement.setPosition(startX - columnSpacing, startY);
    this->window.draw(textElement);

    textElement.setString("Username");
    textElement.setPosition(startX, startY);
    this->window.draw(textElement);

    textElement.setString("Score");
    textElement.setPosition(startX + columnSpacing, startY);
    this->window.draw(textElement);

    // Display leaderboard entries
    startY += 30;
    int position = 1;
    for (const auto& entry : scores) {
        if (position > 15) break;

        textElement.setCharacterSize(24);
        textElement.setFillColor(position <= 3 ? sf::Color::Red : sf::Color::White);
        textElement.setString(std::to_string(position));
        textElement.setPosition(startX - columnSpacing, startY);
        this->window.draw(textElement);

        textElement.setString(entry.first);
        textElement.setPosition(startX, startY);
        this->window.draw(textElement);

        textElement.setString(std::to_string(entry.second));
        textElement.setPosition(startX + columnSpacing, startY);
        this->window.draw(textElement);

        startY += 30;
        position++;
    }

    // Instructions for navigation
    textElement.setString("Press ESC to return to the menu");
    textElement.setCharacterSize(20);
    textElement.setFillColor(sf::Color::Yellow);
    textElement.setPosition(this->window.getSize().x / 2.0f - textElement.getLocalBounds().width / 2, startY + 20);
    this->window.draw(textElement);

    this->window.display();

    sf::Event event;
    while (this->window.isOpen()) {
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    this->window.clear();  // Clear the window
                    currentState = GameState::MENU;  // Return to the menu state
                    return;
                }
            }
        }
        sf::sleep(sf::milliseconds(0.1));  // Small delay to prevent high CPU usage
    }
}





Game::Game() : playerCar("../assets/player_car.png"), speedFactor(1000.0f),speedIncreaseTimer(0.0f), obstacleSpawnTimer(0.0f), powerUpSpawnTimer(0.0f), obstacleSpawnInterval(3.0f), powerUpSpawnInterval(10.0f) , score{0}, highScore {0} , minDistance{100.0f} {
    this->initWindow();
   if (!obstacleTexture1.loadFromFile("../assets/obstacle1.png")) {
        std::cerr << "Failed to load obstacle1 texture" << std::endl;
    }
    if (!obstacleTexture2.loadFromFile("../assets/obstacle2.png")) {
        std::cerr << "Failed to load obstacle2 texture" << std::endl;
    }
    if (!obstacleTexture3.loadFromFile("../assets/obstacle3.png")) {
        std::cerr << "Failed to load obstacle3 texture" << std::endl;
    }
    if (!powerUpTexture.loadFromFile("../assets/powerup.png"))
    {
        std::cerr << "Failed to load power up texture" << std::endl;
    }
    if (!roadTexture.loadFromFile("../assets/road.png"))
    {
        std::cerr << "Failed to load road texture" << std::endl;
    }
    if (!font.loadFromFile("../assets/ARIAL.TTF")) { // Load a font for text
    std::cerr << "Failed to load font" << std::endl;}
    if (!drivingMusic.openFromFile("../assets/driving.mp3")) {
    std::cerr << "Error loading music file!" << std::endl;
}


scoreText.setFont(font);
scoreText.setCharacterSize(24);
scoreText.setFillColor(sf::Color::White);
scoreText.setPosition(9.f, 9.f);

highScoreText.setFont(font);
highScoreText.setCharacterSize(24);
highScoreText.setFillColor(sf::Color::White);
highScoreText.setPosition(10.f, 40.f);
 howToPLaymain.setFont(font);
    howToPLaymain.setString(
    "\t\t\t\t Welcome to Motion Racer!\n\n"
    "Controls:\n"
    "- Use the Arrow keys to Control:\n"
    "  Up Arrow: Move forward\n"
    "  Down Arrow: Slow down\n"
    "  Left Arrow: Steer left\n"
    "  Right Arrow: Steer right\n"
    "Geusture Mode :\n"
    "- Use the Thumb Direction to navigate:\n"
    "  Thumbs Up : Move forward\n"
    "  Thumbs Down : Slow down\n"
    "  Thumbs Left: Steer left\n"
    "  Thumbs Right: Steer left\n\n"
    "Objective:\n"
    "- Avoid obstacles, take power-ups, and survive as long as you can!\n"
    "Tips:\n"
    "- Stay alert and control your speed! \n"
    "- Collect power-ups to boost your performance. \n"
    "Use the arrow keys to navigate through the tutorial: \n"
    "Esc: Previous \n"
    "Enter: Next \n"
    "Up arrow : Up \n"
    "Down arrow : Down \n"
    "Good luck on the road! "
    );
    howToPLaymain.setCharacterSize(18);
    howToPLaymain.setFillColor(sf::Color::White);
    howToPLaymain.setPosition(50.f, 30.f);

loadHighScore(); 
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
    
   

    
    scoreText.setFont(font);
    std::string score_str = std::to_string(score);
    scoreText.setString("Score: " + score_str);
    scoreText.setCharacterSize(48);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(9.f, 9.f);

}
void Game::loadHighScore() {
    std::ifstream file("highscore.txt",std::ios::in);
    if (file.is_open()) {
        file >> highScore;
        std::cout << "High Score Loaded: " << highScore << std::endl;
        file.close();
    }
    else {
        highScore = 0;
        std::cout << "No high score file found. Starting with high score: " << highScore << std::endl;
    }
}

void Game::saveHighScore(const std::string& username) {
    std::ofstream file("../assets/highscore.dat", std::ios::app); // Append mode
    if (file.is_open()) {
        file << username << " " << highScore << std::endl; // Save username and score
        std::cout << "High Score Saved: " << username << " " << highScore << std::endl;
        file.close();
    } else {
        std::cerr << "Failed to open highscore.dat for saving." << std::endl;
    }
}


void Game::initWindow() {
    this->window.create(sf::VideoMode(800, 600), "MotionRacer");
}

void Game::processEvents() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            resetHighScore();
            this->window.close();
        }
        // Only detect Escape key press if the current state is PLAYING
        else if (currentState == GameState::PLAYING && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            currentState = GameState::PAUSED;
        }
    }
}
void Game::resetHighScore() {
    highScore = 0;
    saveHighScore(username);
    std::cout << "High Score reset to zero and saved." << std::endl;
}


void Game::update(float deltaTime) {
     scoreTimer += deltaTime;
    speedIncreaseTimer += deltaTime;  // Increment speed increase timer

    // Increase score every 5 seconds
    if (scoreTimer >= 5.0f) {
        score += 1;
        scoreTimer = 0.0f;
    }

    if (speedIncreaseTimer >= 10.0f) {
        speedFactor += 100.0f;  
        speedIncreaseTimer = 0.0f; 
    }

    playerCar.update(deltaTime);

    roadSprite1.move(0, 100 * deltaTime);
    roadSprite2.move(0, 100 * deltaTime);

    std::string score_str = std::to_string(score);
    scoreText.setString(score_str);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(9.f, 9.f);


    obstacleSpawnTimer +=  3.5*deltaTime;
    powerUpSpawnTimer += 1 * deltaTime;

    spawnObstacles(deltaTime);
    spawnPowerUps(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            if (drivingMusic.getStatus() == sf::Music::Playing) {
                drivingMusic.pause();
            }
        }


    for (auto it = activeObstacles.begin(); it != activeObstacles.end();) {
        it->update(deltaTime);
        if (it->ifOffScreen()) {
            it = activeObstacles.erase(it);
            score++; 
        } else {
            ++it;
        }
    }

    for (auto it = activePowerUps.begin(); it != activePowerUps.end();) {
        if (it->update(deltaTime)) {
            it = activePowerUps.erase(it); 
        } else {
            ++it;
        }
    }

    if (playerCar.checkCollision(activeObstacles)) {
        this->window.clear();
        if (score >= highScore) {
            highScore = score;
        }
        currentState = GameState::GAME_OVER;
    }

    if (playerCar.checkCollision(activePowerUps)) {
        score += 10; // Increment score by 2 for each power-up collected
    }
}

void Game::updateScoreDisplay() {
    scoreText.setString("Score: " + std::to_string(score));
    highScoreText.setString("Final Score: " + std::to_string(highScore));
    std::cout << "Displayed Score: " << scoreText.getString().toAnsiString() << std::endl;
    std::cout << "Displayed High Score: " << highScoreText.getString().toAnsiString() << std::endl;
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
      this->window.draw(scoreText); // Draw current score
  this->window.draw(highScoreText); // Draw high score 
    
}
void Game::spawnObstacles(float deltaTime) {
    if (obstacleSpawnTimer >= obstacleSpawnInterval) {
        bool validSpawn = true;
        sf::Vector2f newPosition(rand() % 700, -50);  // Random X position, Y just above the scree

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
            // Choose a random texture from the loaded textures
            int textureIndex = rand() % 3;  // Assuming you have 3 textures
            sf::Texture* selectedTexture = nullptr;

            // Select the corresponding texture based on the random index
            if (textureIndex == 0) {
                selectedTexture = &obstacleTexture1;
            } else if (textureIndex == 1) {
                selectedTexture = &obstacleTexture2;
            } else {
                selectedTexture = &obstacleTexture3;
            }

            // Create an obstacle using the selected texture
            Obstacle obstacle(*selectedTexture);
            obstacle.setPosition(newPosition);
            activeObstacles.push_back(obstacle);

            // Reset the spawn timer
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
    speedFactor = 1000.0f;
}
float Game::distance(sf::Vector2f pos1, sf::Vector2f pos2)
{
    return sqrt(pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2));
}
void Game::run() {
    std::string username = getUsername();
    closeWindow();
    if (usernameEntered) {
        usernameEntered = false;
        sf::sleep(sf::seconds(0.2));
    }

    sf::Event event;
    bool keyPressed = false;
    currentState = GameState::MENU;
    initializeGameWindow();
    loadHighScore();
    sf::Clock game_clock;
    Menu settings(1);
    keyPressed = false;
    GameState previousState = GameState::MENU;  // Track the previous state for return on Escape

    while (this->window.isOpen()) {
        float deltaTime = frame_clock.restart().asSeconds();
        this->window.clear();
        this->processEvents();

        switch (currentState) {
        case GameState::MENU: {
    bool enterPressed = menu.updateMenu(this->window);
    menu.drawMenu(this->window);
    if (enterPressed) {
        int selected = menu.getClickedItem();
        if (selected == 0) {
            currentState = GameState::PLAYING;
        } else if (selected == 1) {
            currentState = GameState::HOW_TO_PLAY;
        } else if (selected == 2) {
            currentState = GameState::SETTINGS;
        } else if (selected == 3) {
            currentState = GameState::HIGH_SCORE;
        } else if (selected == 5) {
            this->window.close();
        } else if (selected == 4) {
            currentState = GameState::CREDITS;
        }
    }
    break;
}

        case GameState::PLAYING: {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                if (drivingMusic.getStatus() == sf::Music::Playing) {
                    drivingMusic.pause();
                }
                currentState = GameState::PAUSED;
            }

            if (drivingMusic.getStatus() != sf::Music::Playing) {
                drivingMusic.play();
            }
            this->update(deltaTime);
            this->render();
            break;
        }
        case GameState::CREDITS: {
    // Title "Created by" in bold red
    sf::Text createdByText;
    createdByText.setFont(font);
    createdByText.setString("Created by");
    createdByText.setCharacterSize(48);
    createdByText.setFillColor(sf::Color::Red);
    createdByText.setStyle(sf::Text::Bold);
    createdByText.setPosition(window.getSize().x / 2.0f - createdByText.getGlobalBounds().width / 2.0f, 100);

    // Team members' names below
    sf::Text namesText;
    namesText.setFont(font);
    namesText.setString("\n\n Abhishek Panthee\n\n Sulav Paudel\n\n Rhythm Bhetwal \n\n Sushil Bhatta");
    namesText.setCharacterSize(32);
    namesText.setFillColor(sf::Color::Blue);
    namesText.setPosition(window.getSize().x / 2.0f - namesText.getGlobalBounds().width / 2.0f, 150);

    // Draw the credits
    window.draw(createdByText);
    window.draw(namesText);

    // Return to menu on escape key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        currentState = GameState::MENU;
    }

    break;
}


       case GameState::HOW_TO_PLAY: {
    // Display the "How to Play" instructions text
    sf::Text howToPlayMain;
    howToPlayMain.setFont(font);
    howToPlayMain.setString(
        " \t\t\t\t Welcome to Motion Racer\n\n"
        "Controls:\n"
        "- Use the Arrow keys to Control:\n"
        "  Up Arrow: Move forward\n"
        "  Down Arrow: Slow down\n"
        "  Left Arrow: Steer left\n"
        "  Right Arrow: Steer right\n"
        "Gesture Mode:\n"
        "- Use the Thumb Direction to navigate:\n"
        "  Thumbs Up : Move forward\n"
        "  Thumbs Down : Slow down\n"
        "  Thumbs Left: Steer left\n"
        "  Thumbs Right: Steer right\n\n"
        "Objective:\n"
        "- Avoid obstacles, take power-ups, and survive as long as you can!\n"
        "Tips:\n"
        "- Stay alert and control your speed! \n"
        "- Collect power-ups to boost your performance. \n"
        "Use the arrow keys to navigate through the tutorial: \n"
        "Esc: Previous \n"
        "Enter: Next \n"
        "Up arrow : Up \n"
        "Down arrow : Down \n"
        "Good luck on the road!"
    );
    howToPlayMain.setCharacterSize(18);
    howToPlayMain.setFillColor(sf::Color::White);
    howToPlayMain.setPosition(50.f, 30.f);

    // Draw the main instructions text to the screen
    window.draw(howToPlayMain);

    // Event handling
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                // Change to the previous state (for example, back to PAUSED)
                currentState = GameState::MENU;
                break;
            }
            // Handle other key presses (for navigation or progressing through the tutorial, if needed)
        }
        else if (event.type == sf::Event::Closed) {
            this->window.close();
            break;
        }
    }
    break;
}




case GameState::PAUSED: {
    howToPlayClicked = false;
     if (drivingMusic.getStatus() == sf::Music::Playing) {
                drivingMusic.pause();
            }

    displayResumeButton();
        sf::Text howToPlayMain;
        howToPlayMain.setFont(font);
        howToPlayMain.setString(
            "\n\n"
            " \t So you are confused ? "
            " See how Controls work :\n"
            "- Use the Arrow keys to Control:\n"
            "  Up Arrow: Move forward\n"
            "  Down Arrow: Slow down\n"
            "  Left Arrow: Steer left\n"
            "  Right Arrow: Steer right\n"
            " See how Gesture Mode Work :\n"
            "- Use the Thumb Direction to navigate:\n"
            "  Thumbs Up : Move forward\n"
            "  Thumbs Down : Slow down\n"
            "  Thumbs Left: Steer left\n"
            "  Thumbs Right: Steer right\n\n"
            "Objective:\n"
            "- Avoid obstacles, take power-ups, and survive as long as you can!\n"
            "Tips:\n"
            "- Stay alert and control your speed! \n"
            "- Collect power-ups to boost your performance. \n"
            "- Now click on the Resume button and concour the game \n"
            
        );
        howToPlayMain.setCharacterSize(18);
        howToPlayMain.setFillColor(sf::Color::White);
        howToPlayMain.setPosition(50.f, 30.f);

        // Draw the main instructions text to the screen
        window.draw(howToPlayMain);
    
    if (isResumeButtonClicked()) {
        currentState = GameState::PLAYING;
    }

    sf::sleep(sf::milliseconds(20)); // Prevent multiple clicks in quick succession
    break;
}
        case GameState::GAME_OVER: {
            if (drivingMusic.getStatus() == sf::Music::Playing) {
                drivingMusic.stop();
            }

            std::cout << "Game over!" << std::endl;
            std::cout << "Score: " << score << std::endl;

            this->window.clear();
            this->window.draw(gameOverText);

            sf::Text usernameText;
            usernameText.setFont(font);
            std::string displayText = "Username            Score\n" + username + std::string(40 - username.size(), ' ') + std::to_string(score);
            usernameText.setString(displayText);
            usernameText.setPosition(100, 200);

            this->window.draw(usernameText);
            this->window.display();

            if (score >= highScore) {
                highScore = score;
                saveHighScore(username);
            }
            displayLeaderboard(currentState);

            this->reset_game();

            while (!keyPressed) {
                while (this->window.pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                        keyPressed = true;
                        break;
                    } else if (event.type == sf::Event::Closed) {
                        this->window.close();
                        keyPressed = true;
                    }
                }
            }

            sf::sleep(sf::milliseconds(2));
            currentState = GameState::MENU;
            break;
        }

        case GameState::SETTINGS: {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                currentState = GameState::MENU;  // Return to MENU on Escape
            }

            bool enterPressed = settings.updateMenu(this->window);
            settings.drawMenu(this->window);
            break;
        }

        case GameState::HIGH_SCORE: {
            displayLeaderboard(currentState);
            break;
        }
        }

        this->window.display();
    }
}
