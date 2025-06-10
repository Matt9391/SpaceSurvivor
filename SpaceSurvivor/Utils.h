#include <vector>
#include <random>
#include <string>
#include <SFML/Graphics.hpp>

#pragma once
class Utils
{
public:

	static int randomInt(int min, int max);
	static unsigned int randomUInt(unsigned int min, unsigned int max);
	static float randomFloat(float min, float max);

	static sf::Clock clock;
	static double dt;
	static void updateDt();
	static float distance(float x1, float y1, float x2, float y2);
	static float distance(sf::Vector2f a, sf::Vector2f b);
	static int constrain(int val, int min, int max);
	static float constrain(float val, float min, float max);
	static float map(float value, float inMin, float inMax, float outMin, float outMax);
	static sf::Vector2f normalize(const sf::Vector2f& v);
	static float getSign(float value);

	static sf::Vector2f limitMagnitude(sf::Vector2f v, float max);
	static sf::Vector2f setMagnitude(sf::Vector2f v, float newMag);
	static float getMagnitude(sf::Vector2f v);

	static float degreesToRadians(float degrees);
	static float radiansToDegrees(float radians);

	static float PI;
private:
	static std::random_device rd;
	static std::mt19937 gen;
};

