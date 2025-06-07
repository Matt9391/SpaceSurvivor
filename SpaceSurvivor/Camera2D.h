#include <SFML/Graphics.hpp>


#pragma once
class Camera2D
{
public:
	Camera2D(sf::Vector2f size, float ease);

	void setCenter(sf::Vector2f position);

	void follow(sf::Vector2f position, float dt);

	sf::View camera;

private:
	float ease;
};

