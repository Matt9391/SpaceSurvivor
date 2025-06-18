#include <SFML/Graphics.hpp>
#include <vector>

#pragma once
class Background
{
public:
	//Class that moves the background to create an effect of infine world
	Background(sf::Texture& text, sf::Vector2f center);

	void display(sf::RenderWindow& window);
	void update(sf::Vector2f spaceshipPos);
private:

	//number of backgrounds
	int nBgs;
	//size of each background
	sf::Vector2f size;
	std::vector<sf::Sprite> backgrounds;

};

