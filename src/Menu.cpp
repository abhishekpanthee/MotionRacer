#include "Menu.h"
#include<iostream>

Menu::Menu()
{
	if (!font.loadFromFile("ARIAL.TTF"))
	{
		std::cout << "Font not found";
	}
	int width = 300;
	int height = 400;

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Start");
	menu[0].setPosition(sf::Vector2f(width / 2, height / 5 * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("How to play");
	menu[1].setPosition(sf::Vector2f(width / 2, height / 5 * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Settings");
	menu[2].setPosition(sf::Vector2f(width / 2, height / 5 * 3));

	menu[3].setFont(font);
	menu[3].setFillColor(sf::Color::White);
	menu[3].setString("Exit");
	menu[3].setPosition(sf::Vector2f(width / 2, height / 5 * 4));

	selectedIndex = 0;
}

bool Menu::updateMenu(sf::RenderWindow& window)
{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			moveUp();
			sf::sleep(sf::milliseconds(150));
		}
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			moveDown();
			sf::sleep(sf::milliseconds(150));
		}
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			return true;
		}
		return false;

	//sf::Event event;
	//while (window.pollEvent(event))
	//{
	//	// check the type of the event...
	//	switch (event.type)
	//	{
	//	case sf::Event::KeyPressed:
	//		switch (event.key.code)
	//		{
	//		case sf::Keyboard::Up:
	//			moveUp();
	//			break;
	//		case sf::Keyboard::Down:
	//			moveDown();
	//			break;
	//		case sf::Keyboard::Enter:
	//			return true;

	//		default:
	//			break;
	//		}
	//	}
	//}
	//return false;
		
}

void Menu::drawMenu(sf::RenderWindow& window)
{
	for (int i = 0; i < 4; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::moveUp()
{
	if (selectedIndex - 1 >= 0)
	{
		menu[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex--;
		menu[selectedIndex].setFillColor(sf::Color::Red);
	}
}

void Menu::moveDown()
{
	if (selectedIndex + 1 < 4)
	{
		menu[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex++;
		menu[selectedIndex].setFillColor(sf::Color::Red);
	}
}

int Menu::getClickedItem()
{
	return selectedIndex;
}



