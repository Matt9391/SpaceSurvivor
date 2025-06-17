#include "SpaceShip.h"
#include "Utils.h"
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"

SpaceShip::SpaceShip(sf::Vector2f position,sf::Vector2f centerOfRotation, sf::Texture &text, sf::Texture& bulletText) :
	position(position),
	centerOfRotation(centerOfRotation),
	size(text.getSize().x * 0.7f, text.getSize().y * 0.8f),
	speed(50),
	velocity(sf::Vector2f(0,0)),
	acceleration(sf::Vector2f(0,0)),
	angleRotation(0),
	bulletText(bulletText),
	canShoot(true),
	shootCooldown(0.02f),
	movementMode(false)
	{
		
		this->hitbox.setSize(sf::Vector2f(this->size.x, this->size.y));
		this->hitbox.setOrigin(this->size.x / 2.f, this->size.y / 2.f);
		this->hitbox.setFillColor(sf::Color::Blue);

		this->gfx.setTexture(text);
		this->gfx.setScale(1,1);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);

		this->shootTimer.restart();
	}

sf::Vector2f SpaceShip::getPosition() {
	return this->position;
}
sf::Vector2f SpaceShip::getVelocity() {
	return this->velocity;
}

std::vector<Bullet>& SpaceShip::getBullets() {
	return this->bullets;
}

void SpaceShip::setMode(bool mode) {
	this->movementMode = mode;
}

sf::Vector2f SpaceShip::getSize() {
	return sf::Vector2f((float)this->gfx.getTexture()->getSize().x, (float)this->gfx.getTexture()->getSize().y);
}

void SpaceShip::update(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt) {
	this->handleMovement(mousePosition, spaceshipPos, dt);

	if ((this->shootTimer.getElapsedTime()).asSeconds() > this->shootCooldown) {
		this->canShoot = true;
	}

	if (this->canShoot && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		bullets.push_back(Bullet(this->position, 70, this->angleRotation - 90.f, this->bulletText));
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

void SpaceShip::handleMovement(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt) {
	this->acceleration = sf::Vector2f(0, 0);

	this->angleRotation = Utils::radiansToDegrees(atan2(mousePosition.y - spaceshipPos.y, mousePosition.x - spaceshipPos.x)) + 90.f;
	
	if (this->movementMode) {
		float angle = 0;
		int dy = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			dy = 1;
		}

		if (dy != 0) {
			float radAngle = Utils::degreesToRadians(this->angleRotation) - Utils::PI / 2.f;
			this->acceleration = sf::Vector2f(this->speed * cos(radAngle) * dt, this->speed * sin(radAngle) * dt * dy);
		}
		else {
			this->velocity *= 0.96f;
		}
	}
	else {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			float radAngle = Utils::degreesToRadians(this->angleRotation) - Utils::PI / 2.f;
			this->acceleration = sf::Vector2f(this->speed * cos(radAngle), this->speed * sin(radAngle));
			this->acceleration = Utils::normalize(this->acceleration) * this->speed * dt;

		}
		else {
			this->velocity *= 0.96f;
		}
	}

	this->velocity += this->acceleration;
	float maxSpeed = 50.f;
	this->velocity = Utils::limitMagnitude(this->velocity, maxSpeed);

	//std::cout << mousePosition.x << " angle " << this->angleRotation << std::endl;

	this->position += this->velocity;
}

void SpaceShip::display(sf::RenderWindow& window) {
	this->hitbox.setPosition(this->position);
	this->gfx.setPosition(this->position);
	this->gfx.setRotation(this->angleRotation);

	for (Bullet& b : this->bullets) {
		b.display(window);
	}

	//window.draw(this->hitbox);
	window.draw(this->gfx);

	ImGui::Text("bullets: %d", this->bullets.size());
}