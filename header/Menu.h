#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

class Menu {

private: 
	int selectedIndex;
	sf::Text menu[5];
	sf::Text settings[2];
	sf::Font font;
	bool active = true;


public: 
	Menu();
	Menu(int a);
    	bool updateMenu(sf::RenderWindow& window);
	void drawMenu(sf::RenderWindow& window);

	void moveUp();
	void moveDown();
	int getClickedItem();

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactivate() { active = false; }



};
