#include <SFML/Graphics.hpp>
#include <vector>

#pragma once
class Background
{
public:
	Background(sf::Texture& text, sf::Vector2f center);

	void display(sf::RenderWindow& window);
	void update(sf::Vector2f spaceshipPos);
private:

	int nBgs;
	sf::Vector2f size;
	std::vector<sf::Sprite> backgrounds;

};

