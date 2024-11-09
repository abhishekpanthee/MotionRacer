#include "../header/Menu.h"
#include<iostream>

Menu::Menu() : size{6}
{
	if (!font.loadFromFile("../assets/ARIAL.TTF"))
	{
		std::cout << "Font not found";
	}
	int width = 300;
	int height = 400;

	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Start");
	menu[0].setPosition(sf::Vector2f(width / 2, height / 6 * 1));

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("How to play");
	menu[1].setPosition(sf::Vector2f(width / 2, height / 6 * 2));

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Settings");
	menu[2].setPosition(sf::Vector2f(width / 2, height / 6 * 3));

	menu[3].setFont(font);
	menu[3].setFillColor(sf::Color::White);
	menu[3].setString("High Score");
	menu[3].setPosition(sf::Vector2f(width / 2, height / 6 * 4));

	menu[5].setFont(font);
	menu[5].setFillColor(sf::Color::White);
	menu[5].setString("Exit");
	menu[5].setPosition(sf::Vector2f(width / 2, height / 6 * 6));
	menu[4].setFont(font);
	menu[4].setFillColor(sf::Color::White);
	menu[4].setString("Credits");
	menu[4].setPosition(sf::Vector2f(width / 2, height / 6 * 5));
	size = 6; 
	selectedIndex = 0;
	footerText.setFont(font);
    footerText.setFillColor(sf::Color::Yellow);
    footerText.setCharacterSize(18); 
    footerText.setString("Created by  \n Abhishek Panthee \n Sulav Paudel \n Rhythem Bhetwal \n and Sushil Bhatta");

    // Center align footer text at the bottom
    sf::FloatRect textRect = footerText.getLocalBounds();
    footerText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    footerText.setPosition(sf::Vector2f(width /1.5 , height / 6 *8 ));
}

Menu::Menu(int a):selectedIndex{0} , size{2}
{
	if (!font.loadFromFile("../assets/ARIAL.TTF"))
	{
		std::cout << "Font not found";
	}
	int width = 300;
	int height = 400;

	settings[0].setFont(font);
	settings[0].setFillColor(sf::Color::Red);
	settings[0].setString("Music");
	settings[0].setPosition(sf::Vector2f(width / 2, height / 3 * 1));

	settings[1].setFont(font);
	settings[1].setFillColor(sf::Color::White);
	settings[1].setString("Gesture Mode");
	settings[1].setPosition(sf::Vector2f(width / 2, height / 3 * 2));

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
		
}

void Menu::drawMenu(sf::RenderWindow& window)
{
	if (size == 6)
{
	for (int i = 0; i < size; i++)
	{
		window.draw(menu[i]);
	}
}
else if (size == 2)
{
	for (int i = 0; i < size; i++)
	{
		window.draw(settings[i]);
	}
}
}

void Menu::moveUp()
{
	if (size == 6)
{
	if (selectedIndex - 1 >= 0)
	{
		menu[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex--;
		menu[selectedIndex].setFillColor(sf::Color::Red);
	}
}
else if (size == 2)
{
	if (selectedIndex - 1 >= 0)
	{
		settings[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex--;
		settings[selectedIndex].setFillColor(sf::Color::Red);
	}
}
}

void Menu::moveDown()
{
	if (size == 6)
{
	if (selectedIndex + 1 < size)
	{
		menu[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex++;
		menu[selectedIndex].setFillColor(sf::Color::Red);
	}
}
else if (size == 2)
{
	if (selectedIndex + 1 < size)
	{
		settings[selectedIndex].setFillColor(sf::Color::White);
		selectedIndex++;
		settings[selectedIndex].setFillColor(sf::Color::Red);
	}
}
	
}

int Menu::getClickedItem()
{
	return selectedIndex;
}



