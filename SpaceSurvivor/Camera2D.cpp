#include "Camera2D.h"

Camera2D::Camera2D(sf::Vector2f size, float ease) :
	ease(ease)
	{
		this->camera.setSize(size);
	};

void Camera2D::setCenter(sf::Vector2f position) {
	this->camera.setCenter(position);
}

void Camera2D::follow(sf::Vector2f position, float dt) {
	sf::Vector2f direction = position - this->camera.getCenter();

	this->camera.move(direction * this->ease * dt);
}