#include <SFML/Graphics.hpp>
#include "SpaceShip.h"
#include <vector>


#pragma once
class Enemy
{
public:
	Enemy(sf::Vector2f position, sf::Texture& text, sf::Texture& bulletText);

	void display(sf::RenderWindow& window);
	void update(SpaceShip &spaceship, float dt);

	void setPosition(sf::Vector2f position);
	void setVelocity(sf::Vector2f velocity);

	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getSize();
	std::vector<Bullet>& getBullets();

	sf::Vector2f evade(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce);
	void addForce(sf::Vector2f force);
	
	void deleteSelf();

	bool toRemove;
private:
	void handleMovement(SpaceShip& spaceship, float dt);
	//https://www.youtube.com/watch?v=Q4MU7pkDYmQ&list=PL0EXn74dzTecIxVyK03chZU0RIajfEApp&index=11
	sf::Vector2f pursue(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce);
	sf::Vector2f seek(sf::Vector2f &target, float maxSteeringForce);
	sf::Vector2f curveEvade(SpaceShip& spaceship, float maxSteeringForce);

	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;

	float maxSpeed;
	float maxSteeringForce;
	float maxSteeringEvadeForce;

	float angleRotation;

	sf::Sprite gfx;

	sf::Clock shootTimer;
	float shootCooldown;
	bool canShoot;

	std::vector<Bullet> bullets;
	sf::Texture bulletText;

	sf::RectangleShape hitbox;


};



