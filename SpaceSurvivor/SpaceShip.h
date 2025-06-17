#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Bullet.h"

#pragma once
class SpaceShip
{
public:
	SpaceShip(sf::Vector2f position, sf::Vector2f centerOfRotation, sf::Texture& text, sf::Texture& bulletText);

	void update(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt);
	void display(sf::RenderWindow& window);

	void setPosition(sf::Vector2f position);
	void setVelocity(sf::Vector2f velocity);
	void setHp(float hp);

	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getSize();
	std::vector<Bullet>& getBullets();
	int getHp();

	void setMode(bool mode);
	void damage(int damage);


private:
	void handleMovement(sf::Vector2f mousePosition, sf::Vector2f spaceshipPos, float dt);


	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float speed;

	sf::RectangleShape hitbox;

	sf::Vector2f centerOfRotation;
	sf::Sprite gfx;

	float angleRotation;

	sf::Clock shootTimer;
	float shootCooldown;
	bool canShoot;

	std::vector<Bullet> bullets;
	sf::Texture bulletText;

	bool movementMode;

	int hp;

	sf::RectangleShape backgroundBar;
	sf::RectangleShape hpBar;

	sf::Sound shootSound;
	sf::SoundBuffer soundBuffer;

	sf::Sprite powerSprite;
	bool moving;

};

