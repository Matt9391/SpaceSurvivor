#include <SFML/Graphics.hpp>


#pragma once
class Camera2D
{
public:
	//this is my life changer ahaha, its just a basic camera that follow the player
	Camera2D(sf::Vector2f size, float ease);

	//instant set position
	void setCenter(sf::Vector2f position);

	//slowly follow a position, spaceShipVelocity used to change the zoom level dynamic
	void follow(sf::Vector2f position, sf::Vector2f spaceShipVelocity, float dt);

	sf::View camera;

private:
	float ease;
	sf::Vector2f size;
};

