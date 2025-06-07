#include "SpaceShip.h"
#include "Utils.h"
#include <iostream>

SpaceShip::SpaceShip(sf::Vector2f position,sf::Vector2f centerOfRotation, sf::Texture &text) :
	position(position),
	centerOfRotation(centerOfRotation),
	size(sf::Vector2f(1, 1)),
	speed(50),
	velocity(sf::Vector2f(0,0)),
	acceleration(sf::Vector2f(0,0)),
	angleRotation(0)
	{
		this->hitbox.setSize(sf::Vector2f(this->size.x, this->size.y));
		this->hitbox.setOrigin(this->size.x / 2.f, this->size.y / 2.f);
		this->hitbox.setFillColor(sf::Color::Blue);

		this->gfx.setTexture(text);
		this->gfx.setScale(this->size.x, this->size.y);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);
	}

sf::Vector2f SpaceShip::getPosition() {
	return this->position;
}

void SpaceShip::update(sf::Vector2f mousePosition, float dt) {
	this->acceleration = sf::Vector2f(0, 0);

	this->angleRotation = Utils::radiansToDegrees(atan2(mousePosition.y - this->centerOfRotation.y, mousePosition.x - this->centerOfRotation.x)) + 90.f;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

		float radAngle = Utils::degreesToRadians(this->angleRotation) - Utils::PI / 2.f;
		this->acceleration = sf::Vector2f(this->speed * cos(radAngle) * dt, this->speed * sin(radAngle) * dt);

	}
	else {
		this->velocity *= 0.96f;
	}

	this->velocity += this->acceleration;
	float maxSpeed = 30.f;
	this->velocity.x = Utils::constrain(this->velocity.x, -maxSpeed, maxSpeed);
	this->velocity.y = Utils::constrain(this->velocity.y, -maxSpeed, maxSpeed);
	
	//std::cout << mousePosition.x << " angle " << this->angleRotation << std::endl;

	this->position += this->velocity;
}

void SpaceShip::display(sf::RenderWindow& window) {
	this->hitbox.setPosition(this->position);
	this->gfx.setPosition(this->position);
	this->gfx.setRotation(this->angleRotation);

	window.draw(this->hitbox);
	window.draw(this->gfx);
}