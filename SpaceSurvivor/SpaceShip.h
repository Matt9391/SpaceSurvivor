#include <SFML/Graphics.hpp>

#pragma once
class SpaceShip
{
public:
	SpaceShip(sf::Vector2f position, sf::Vector2f centerOfRotation, sf::Texture &text);

	void update(sf::Vector2f mousePosition, float dt);
	void display(sf::RenderWindow& window);

	sf::Vector2f getPosition();
private:
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float speed;

	sf::RectangleShape hitbox;

	sf::Vector2f centerOfRotation;
	sf::Sprite gfx;

	float angleRotation;

};

