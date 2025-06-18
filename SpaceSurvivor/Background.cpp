#include "Background.h"
#include <iostream>
#include "Utils.h"


Background::Background(sf::Texture& text, sf::Vector2f center) :
	nBgs(9),
	size(text.getSize())
	{
		//initalize backgrounds
		for (int i = 0; i < this->nBgs; i++) {
			this->backgrounds.push_back(sf::Sprite(text));
			this->backgrounds[i].setOrigin(sf::Vector2f(this->size.x / 2.f, this->size.y / 2.f));
		}

		//set position in a grid 3x3
		int cont = 0;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				this->backgrounds[cont].setPosition(center.x + size.x * i, center.y + size.y * j);
				cont++;
			}
		}

	}

void Background::update(sf::Vector2f spaceshipPos) {
	for (int i = 0; i < this->nBgs; i++) {
		sf::Vector2f bgPos = this->backgrounds[i].getPosition();

		sf::Vector2f dist = spaceshipPos - bgPos;

		//if you are too far away from a bg it moves in the direction of the dist (getSign) of 3 times the size of the bg
		if (std::abs(dist.x) >= this->size.x) {
			float signX = Utils::getSign(dist.x);
			this->backgrounds[i].move(this->size.x * 3.f * signX, 0.f);
		}
		if (std::abs(dist.y) >= this->size.y) {
			float signY = Utils::getSign(dist.y);
			this->backgrounds[i].move(0.f, this->size.y * 3.f * signY);
		}
	}

}

void Background::display(sf::RenderWindow& window) {
	for (int i = 0; i < this->nBgs; i++) {
		window.draw(this->backgrounds[i]);
	}
}