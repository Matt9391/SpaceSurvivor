#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "SpaceShip.h"
#include "Camera2D.h"
//#include "imgui.h"
//#include "imgui-SFML.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML works!");
    window.setFramerateLimit(60);
    //ImGui::SFML::Init(window);

    Camera2D camera(sf::Vector2f(1200, 800), 10);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("./Sprites/player.png");

    sf::Vector2f midScreen(window.getSize().x / 2.f, window.getSize().y / 2.f);
    SpaceShip spaceship(sf::Vector2f(600, 400), midScreen, playerTexture);
    
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(1000, 600));
    rect.setPosition(100, 100);
    rect.setFillColor(sf::Color::Cyan);

    while (window.isOpen())
    {
        Utils::updateDt();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        sf::Vector2f mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        spaceship.update(mousePos, Utils::dt);

        camera.follow(spaceship.getPosition(), Utils::dt);
        //camera.setCenter(spaceship.getPosition());

        window.setView(camera.camera);
        window.clear();
        window.draw(rect);
        spaceship.display(window);
        window.display();
    }

    return 0;
}