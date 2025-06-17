#include <iostream>

#include "SpaceShip.h"
#include "Utils.h"

#include "imgui.h"
#include "imgui-SFML.h"

SpaceShip::SpaceShip(sf::Vector2f position, sf::Vector2f centerOfRotation, sf::Texture& text, sf::Texture& bulletText) :
	position(position),
	centerOfRotation(centerOfRotation),
	size(text.getSize().x * 0.7f, text.getSize().y * 0.8f),
	speed(50),
	velocity(sf::Vector2f(0, 0)),
	acceleration(sf::Vector2f(0, 0)),
	angleRotation(0),
	bulletText(bulletText),
	canShoot(true),
	shootCooldown(0.2f),
	movementMode(false),
	moving(false),
	hp(100)
	{
		
		this->hitbox.setSize(sf::Vector2f(this->size.x, this->size.y));
		this->hitbox.setOrigin(this->size.x / 2.f, this->size.y / 2.f);
		this->hitbox.setFillColor(sf::Color::Blue);

		this->gfx.setTexture(text);
		this->gfx.setScale(1,1);
		this->gfx.setOrigin(text.getSize().x / 2.f, text.getSize().y / 2.f);

		this->shootTimer.restart();

		this->hpBar.setPosition(this->hitbox.getPosition() + sf::Vector2f(0.f, 55.f));
		this->hpBar.setSize(sf::Vector2f(100.f, 10.f));
		this->hpBar.setOrigin(50.f, 5.f);
		this->hpBar.setFillColor(sf::Color::Green);
		this->hpBar.setOutlineColor(sf::Color(40, 40, 40));
		this->hpBar.setOutlineThickness(2.f);

		this->soundBuffer.loadFromFile("./Audio/shootSound.wav");
		this->shootSound.setBuffer(this->soundBuffer);

		this->powerSprite.setTexture(bulletText);
		this->powerSprite.setOrigin(bulletText.getSize().x / 2.f, bulletText.getSize().y / 2.f);
		this->powerSprite.setPosition(this->gfx.getPosition());
		this->powerSprite.setRotation(this->angleRotation);
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

void SpaceShip::damage(int damage) {
	this->hp -= damage;
	this->hp = Utils::constrain(this->hp, 0, 100);
}

int SpaceShip::getHp() {
	return this->hp;
}

void SpaceShip::setPosition(sf::Vector2f position) {
	this->position = position;
}
void SpaceShip::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}
void SpaceShip::setHp(float hp) {
	this->hp = hp;
}

void SpaceShip::update(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt) {
	this->handleMovement(mousePosition, spaceshipPos, dt);

	if ((this->shootTimer.getElapsedTime()).asSeconds() > this->shootCooldown) {
		this->canShoot = true;
	}

	if (this->canShoot && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		bullets.push_back(Bullet(this->position, 70, this->angleRotation - 90.f, this->bulletText));
		this->canShoot = false;
		this->shootSound.play();
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

	float lengthX = Utils::map(this->hp, 0.f, 100.f, 0.f, 100.f);
	this->hpBar.setSize(sf::Vector2f(lengthX, 10.f));


}

void SpaceShip::handleMovement(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt) {
	this->acceleration = sf::Vector2f(0, 0);

	this->angleRotation = Utils::radiansToDegrees(atan2(mousePosition.y - spaceshipPos.y, mousePosition.x - spaceshipPos.x)) + 90.f;
	
	this->moving = false;

	if (this->movementMode) {
		float angle = 0;
		int dy = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			dy = 1;
		}

		if (dy != 0) {
			float radAngle = Utils::degreesToRadians(this->angleRotation) - Utils::PI / 2.f;
			this->acceleration = sf::Vector2f(this->speed * cos(radAngle) * dt, this->speed * sin(radAngle) * dt * dy);
			this->moving = true;
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
			this->moving = true;
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
	this->hpBar.setPosition(this->hitbox.getPosition() + sf::Vector2f(0.f, 50.f));
	float radAngle = Utils::degreesToRadians(this->angleRotation);
	float offsetDistance = 40.f; // distanza dal centro verso il basso
	sf::Vector2f offset = sf::Vector2f(
		0.f - std::sin(radAngle) * offsetDistance,
		0.f + std::cos(radAngle) * offsetDistance
	);
	this->powerSprite.setPosition(this->gfx.getPosition() + offset);
	this->powerSprite.setRotation(this->angleRotation - 90.f);

	for (Bullet& b : this->bullets) {
		b.display(window);
	}

	//window.draw(this->hitbox);
	if (this->moving) {
		window.draw(this->powerSprite);
	}
	window.draw(this->gfx);
	window.draw(this->hpBar);

	ImGui::Text("hps: %d", this->hp);
}