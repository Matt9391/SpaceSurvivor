#include "Bullet.h"
#include "Utils.h"
#include <cmath>
#include <iostream>

Bullet::Bullet(sf::Vector2f position, float speed, float angle, sf::Texture& text) :
	position(position),
	size(text.getSize().x * 0.25f, text.getSize().y * 0.3f),
	speed(speed),
	angle(angle),
	lifespan(1.5f),
	toRemove(false)
	{
		this->hitbox.setSize(sf::Vector2f(this->size.x, this->size.y));
		this->hitbox.setOrigin(this->size.x / 2.f, this->size.y / 2.f);
		this->hitbox.setFillColor(sf::Color::Green);

		this->gfx.setTexture(text);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);
		this->gfx.setRotation(angle);

		float radAngle = Utils::degreesToRadians(angle);

		this->velocity = sf::Vector2f(cos(radAngle) * this->speed, sin(radAngle) * this->speed);

		this->lifetime.restart();
	}

void Bullet::update(float dt) {

	this->position += this->velocity;

	if ((this->lifetime.getElapsedTime()).asSeconds() > this->lifespan) {
		this->toRemove = true;
	}
}

sf::Vector2f Bullet::getPosition() {
	return this->position;
}
sf::Vector2f Bullet::getSize() {
	return this->size;
}

void Bullet::display(sf::RenderWindow& window) {
	this->hitbox.setPosition(this->position);
	this->gfx.setPosition(this->position);
	
	//window.draw(this->hitbox);

	window.draw(this->gfx);
}