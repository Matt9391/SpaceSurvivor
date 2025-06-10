#include "Enemy.h"
#include "Utils.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f position, sf::Texture& text, sf::Texture& bulletText) :
	position(position),
	size(sf::Vector2f(1, 1)),
	acceleration(0,0),
	velocity(0,0),
	maxSpeed(50),
	maxSteeringForce(0.8),
	maxSteeringEvadeForce(1),
	angleRotation(0),
	bulletText(bulletText),
	canShoot(false),
	shootCooldown(1.f)
	{
		this->gfx.setTexture(text);
		this->gfx.setScale(this->size.x, this->size.y);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);

		this->shootTimer.restart();
	}

void Enemy::update(SpaceShip &spaceship, float dt) {
	this->handleMovement(spaceship, dt);
	

	this->angleRotation = Utils::radiansToDegrees(std::atan2(this->velocity.y, this->velocity.x) + Utils::PI / 2.f);

	if ((this->shootTimer.getElapsedTime()).asSeconds() > this->shootCooldown) {
		this->canShoot = true;
	}

	if (this->canShoot) {
		this->bullets.push_back(Bullet(this->position, 70, this->angleRotation - 90.f, this->bulletText));
		this->canShoot = false;
		this->shootTimer.restart();
		
	}

	for (Bullet& b : this->bullets) {
		b.update(dt);
	}

	for (int i = this->bullets.size() - 1; i >= 0; i--) {
		if (this->bullets[i].toRemove) {
			this->bullets.erase(this->bullets.begin() + i);
		}
	}

}
void Enemy::handleMovement(SpaceShip& spaceship, float dt) {
	this->acceleration = sf::Vector2f(0, 0);
	if (Utils::distance(spaceship.getPosition(), this->position) < this->gfx.getTexture()->getSize().x * 2.f) {
		this->acceleration += this->curveEvade(spaceship, this->maxSteeringEvadeForce);
		this->velocity = Utils::limitMagnitude(this->velocity, this->maxSpeed / 2.f);
	}
	else {
		this->acceleration += this->pursue(spaceship.getPosition(), spaceship.getVelocity(), this->maxSteeringForce);
	}

	this->velocity += this->acceleration;
	this->velocity = Utils::limitMagnitude(this->velocity, this->maxSpeed);

	this->position += this->velocity ;
}

void Enemy::addForce(sf::Vector2f force) {
	this->acceleration += force;
}

sf::Vector2f Enemy::curveEvade(SpaceShip& spaceship, float maxSteeringForce) {
    sf::Vector2f toTarget = spaceship.getPosition() - this->position;

	//90 degrees to the right to evade the spaceship
	sf::Vector2f evadeForce = sf::Vector2f(-toTarget.y, toTarget.x); 

    evadeForce = Utils::setMagnitude(evadeForce, this->maxSpeed / 1.5f);
	evadeForce -= this->velocity;

	evadeForce = Utils::limitMagnitude(evadeForce, maxSteeringForce);

    return evadeForce;
}

sf::Vector2f Enemy::evade(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce) {
	return this->pursue(position, velocity, maxSteeringForce) * -10.f;
}

sf::Vector2f Enemy::pursue(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce) {
	position += (velocity * 25.f);

	return this->seek(position, maxSteeringForce);
}

sf::Vector2f Enemy::seek(sf::Vector2f &target, float maxSteeringForce) {
	sf::Vector2f force = target - this->position;
	force = Utils::setMagnitude(force, this->maxSpeed);
	force -= this->velocity;
	force = Utils::limitMagnitude(force, maxSteeringForce);

	return force;
}

void Enemy::setPosition(sf::Vector2f position) {
	this->position = position;
}
void Enemy::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}

sf::Vector2f Enemy::getPosition() {
	return this->position;
}
sf::Vector2f Enemy::getVelocity() {
	return this->velocity;
}
sf::Vector2f Enemy::getSize() {
	return sf::Vector2f(this->gfx.getTexture()->getSize().x, this->gfx.getTexture()->getSize().y);
}

void Enemy::display(sf::RenderWindow& window) {
	this->gfx.setPosition(this->position);
	this->gfx.setRotation(this->angleRotation);

	for (Bullet& b : this->bullets) {
		b.display(window);
	}

	window.draw(this->gfx);
}