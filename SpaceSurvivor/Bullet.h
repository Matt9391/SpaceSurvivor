#include <SFML/Graphics.hpp>

#pragma once
class Bullet
{
public:
	Bullet(sf::Vector2f position, float speed, float angle, sf::Texture &text);

	void update(float dt);

	void display(sf::RenderWindow& window);
	
	sf::Vector2f getPosition();
	sf::Vector2f getSize();


	bool toRemove;
private:
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f velocity;
	float angle;
	float speed;

	float lifespan;
	sf::Clock lifetime;

	sf::Sprite gfx;

	sf::RectangleShape hitbox;
};

