#include "Bullet.h"
#include "Utils.h"
#include <cmath>

Bullet::Bullet(sf::Vector2f position, float speed, float angle, sf::Texture& text) :
	position(position),
	speed(speed),
	angle(angle),
	lifespan(5.f),
	toRemove(false)
	{
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

void Bullet::display(sf::RenderWindow& window) {
	this->gfx.setPosition(this->position);
	window.draw(this->gfx);
}