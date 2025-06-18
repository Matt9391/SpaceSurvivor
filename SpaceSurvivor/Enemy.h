#include <SFML/Graphics.hpp>
#include "SpaceShip.h"
#include <vector>


#pragma once
class Enemy
{
public:
	Enemy(sf::Vector2f position, bool freeze, sf::Texture& text, sf::Texture& bulletText);

	void update(SpaceShip &spaceship, float dt);
	void display(sf::RenderWindow& window);

	//setter
	void setPosition(sf::Vector2f position);
	void setVelocity(sf::Vector2f velocity);

	//getter
	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getSize();
	std::vector<Bullet>& getBullets();

	//other functions
	//avoid a dynamic target
	sf::Vector2f evade(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce);
	void addForce(sf::Vector2f force);
	void deleteSelf();
	
	//ik its not that right to have this variabile public but i'm used to it, i dont wanna do another get/set
	bool toRemove;
private:
	//funcion to clean the code
	void handleMovement(SpaceShip& spaceship, float dt);
	//all steering behaivour learned here
	//https://www.youtube.com/watch?v=Q4MU7pkDYmQ&list=PL0EXn74dzTecIxVyK03chZU0RIajfEApp&index=11
	//follow a dynamic target
	sf::Vector2f pursue(sf::Vector2f position, sf::Vector2f velocity, float maxSteeringForce);
	//follow a static target
	sf::Vector2f seek(sf::Vector2f &target, float maxSteeringForce);
	//this is not really a steering behaivour, its more like a evasive dash based on seek behaivour
	sf::Vector2f curveEvade(SpaceShip& spaceship, float maxSteeringForce);

	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;

	float maxSpeed;
	float maxSteeringForce;
	//this one for the evasive dash
	float maxSteeringEvadeForce;

	float angleRotation;

	sf::Sprite gfx;
	sf::RectangleShape hitbox;

	sf::Clock shootTimer;
	float shootCooldown;
	bool canShoot;

	std::vector<Bullet> bullets;
	sf::Texture bulletText;



};



