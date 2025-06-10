#include "Camera2D.h"
#include <iostream>
#include "Utils.h"

Camera2D::Camera2D(sf::Vector2f size, float ease) :
	ease(ease),
	size(size)
	{
		this->camera.setCenter(size.x / 2.f, size.y / 2.f);
		this->camera.setSize(size);
	};

void Camera2D::setCenter(sf::Vector2f position) {
	this->camera.setCenter(position);
}


void Camera2D::follow(sf::Vector2f position, sf::Vector2f spaceShipVelocity, float dt) {
	sf::Vector2f direction = position - this->camera.getCenter();
	
	float mag = Utils::getMagnitude(spaceShipVelocity);
	//std::cout << mag << std::endl;
	mag = Utils::constrain(mag, 0.f, 50.f);
	float newSizeX = Utils::map(mag, 0.f, 50.f, this->size.x, this->size.x * 1.3f);
	float newSizeY = Utils::map(mag, 0.f, 50.f, this->size.y, this->size.y * 1.3f);

	this->camera.setSize(newSizeX,newSizeY);
	this->camera.move(direction * this->ease * dt);
}