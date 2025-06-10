#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::randomInt(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

float Utils::randomFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}

unsigned int Utils::randomUInt(unsigned int min, unsigned int max) {
	std::uniform_int_distribution<unsigned int> dist(min, max);
	return dist(gen);
}


double Utils::dt = 0;
sf::Clock Utils::clock;

void Utils::updateDt() {
	dt = clock.restart().asSeconds();
}

float Utils::distance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float Utils::distance(sf::Vector2f a, sf::Vector2f b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int Utils::constrain(int val, int min, int max) {
	if (val < min) {
		val = min;
	}

	if (val > max) {
		val = max;
	}

	return val;
}
float Utils::constrain(float val, float min, float max) {
	if (val < min) {
		val = min;
	}

	if (val > max) {
		val = max;
	}

	return val;
}

float Utils::map(float value, float inMin, float inMax, float outMin, float outMax) {
	return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

sf::Vector2f Utils::normalize(const sf::Vector2f& v) {

	float length = std::sqrt(v.x * v.x + v.y * v.y);
	if (length != 0) {
		return v / length;
	}else{
		return sf::Vector2f(0.f, 0.f);
	}

}

float Utils::getSign(float value) {
	if (value < 0) {
		return -1.f;
	}

	return 1.f;
}

sf::Vector2f Utils::limitMagnitude(sf::Vector2f v, float max) {
	float mag = std::sqrt(v.x * v.x + v.y * v.y);
	if (mag > max) {
		v /= mag;      // normalizza
		v *= max;      // ridimensiona a lunghezza max
	}
	return v;
}

sf::Vector2f Utils::setMagnitude(sf::Vector2f v, float newMag) {
	float mag = std::sqrt(v.x * v.x + v.y * v.y);
	if (mag == 0.f) return sf::Vector2f(0.f, 0.f); // evita divisione per zero
	return v / mag * newMag;
}

float Utils::getMagnitude(sf::Vector2f v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}


float Utils::PI = 3.14159265f;

float Utils::degreesToRadians(float degrees) {
	return degrees * (PI / 180.f);
}

float Utils::radiansToDegrees(float radians) {
	return radians * (180.f / PI);
}