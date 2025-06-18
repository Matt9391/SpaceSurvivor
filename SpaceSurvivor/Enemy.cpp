#include "Enemy.h"
#include "Utils.h"
#include <iostream>

Enemy::Enemy(sf::Vector2f position, bool freeze, sf::Texture& text, sf::Texture& bulletText) :
	position(position),
	size(sf::Vector2f(text.getSize().x * 0.8f, text.getSize().y * 0.7f)),
	acceleration(0, 0),
	velocity(0, 0),
	maxSpeed(Utils::randomFloat(30.f, 40.f)),
	maxSteeringForce(Utils::randomFloat(0.5f, 0.9f)),
	maxSteeringEvadeForce(1),
	angleRotation(0),
	bulletText(bulletText),
	canShoot(false),
	shootCooldown(1.f),
	toRemove(false)
	{
		this->hitbox.setSize(sf::Vector2f(this->size.x, this->size.y));
		this->hitbox.setOrigin(this->size.x / 2.f, this->size.y / 2.f);
		this->hitbox.setFillColor(sf::Color::Cyan);

		this->gfx.setTexture(text);
		this->gfx.setScale(1, 1);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);

		this->angleRotation = Utils::radiansToDegrees(std::atan2(this->velocity.y, this->velocity.x) + Utils::PI / 2.f);

		this->shootTimer.restart();
	}
//PUBLIC:

//setter
void Enemy::setPosition(sf::Vector2f position) {
	this->position = position;
}
void Enemy::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}

//getter
sf::Vector2f Enemy::getPosition() {
	return this->position;
}
sf::Vector2f Enemy::getVelocity() {
	return this->velocity;
}
sf::Vector2f Enemy::getSize() {
	return this->size;
}
std::vector<Bullet>& Enemy::getBullets() {
	return this->bullets;
}

//others
sf::Vector2f Enemy::evade(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce) {
	return this->pursue(position, velocity, maxSteeringForce) * -20.f;
}

void Enemy::addForce(sf::Vector2f force) {
	this->acceleration += force;
}

void Enemy::deleteSelf() {
	this->bullets.clear();
}

//PRIVATE:

void Enemy::handleMovement(SpaceShip& spaceship, float dt) {
	this->acceleration = sf::Vector2f(0, 0);
	//if the distance to spaceship its less than 2 times the width and the spaceship is basiccaly moving
	if (Utils::distance(spaceship.getPosition(), this->position) < this->size.x * 2.f &&
		Utils::getMagnitude(spaceship.getVelocity()) > 5.f) {
		//apply an avoid dash
		this->acceleration += this->curveEvade(spaceship, this->maxSteeringEvadeForce);
		//limit the velocity in order to avoid that the enemy goes too far away
		this->velocity = Utils::limitMagnitude(this->velocity, this->maxSpeed / 2.f);
	}
	else {
		//otherwise just follow the player
		this->acceleration += this->pursue(spaceship.getPosition(), spaceship.getVelocity(), this->maxSteeringForce);
	}

	this->velocity += this->acceleration;
	this->velocity = Utils::limitMagnitude(this->velocity, this->maxSpeed);

	this->position += this->velocity;
}


sf::Vector2f Enemy::pursue(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce) {
	position += (velocity * 15.f);

	return this->seek(position, maxSteeringForce);
}

sf::Vector2f Enemy::seek(sf::Vector2f& target, float maxSteeringForce) {
	sf::Vector2f force = target - this->position;
	force = Utils::setMagnitude(force, this->maxSpeed);
	force -= this->velocity;
	force = Utils::limitMagnitude(force, maxSteeringForce);

	return force;
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

void Enemy::update(SpaceShip &spaceship, float dt) {
	
	this->handleMovement(spaceship, dt);
	
	this->angleRotation = Utils::radiansToDegrees(std::atan2(this->velocity.y, this->velocity.x) + Utils::PI / 2.f);

	//if the elapsed time its more than the cooldown it shoots
	if ((this->shootTimer.getElapsedTime()).asSeconds() > this->shootCooldown) {
		this->canShoot = true;
	}

	if (this->canShoot) {
		this->bullets.push_back(Bullet(this->position,this->angleRotation - 90.f, this->bulletText));
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


void Enemy::display(sf::RenderWindow& window) {
	this->hitbox.setPosition(this->position);
	this->gfx.setPosition(this->position);
	this->gfx.setRotation(this->angleRotation);

	for (Bullet& b : this->bullets) {
		b.display(window);
	}

	//window.draw(this->hitbox);
	window.draw(this->gfx);
}