#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "SpaceShip.h"
#include "Camera2D.h"
#include "Background.h"
#include "Enemy.h"
//#include "imgui.h"
//#include "imgui-SFML.h"

void handleResize(sf::RenderWindow& window, Camera2D& camera, unsigned int width, unsigned int height);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920,1080), "SFML works!");
    window.setFramerateLimit(60);
    //ImGui::SFML::Init(window);

    Camera2D camera(sf::Vector2f(1920, 1080), 10);

    sf::Texture bgTexture;
    bgTexture.loadFromFile("./Sprites/background2.png");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("./Sprites/bullet1.png");
    sf::Texture bulletTexture2;
    bulletTexture2.loadFromFile("./Sprites/bullet2.png");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("./Sprites/player.png");

    sf::Vector2f midScreen(window.getSize().x / 2.f, window.getSize().y / 2.f);
    SpaceShip spaceship(sf::Vector2f(960, 540), midScreen, playerTexture, bulletTexture);
    Background background(bgTexture, spaceship.getPosition());
    
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("./Sprites/enemy.png");

    std::vector<Enemy> enemies;
    for (int i = 0; i < 20; i++) {
        enemies.push_back(Enemy(sf::Vector2f(Utils::randomFloat(0.f,1200.f), Utils::randomFloat(0.f,800.f)), enemyTexture, bulletTexture2));
    }
    

    while (window.isOpen())
    {
        Utils::updateDt();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                handleResize(window, camera, event.size.width, event.size.height);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        sf::Vector2f mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

        spaceship.update(
            mousePos,
            sf::Vector2f(
                window.mapCoordsToPixel(spaceship.getPosition()).x, 
                window.mapCoordsToPixel(spaceship.getPosition()).y), 
            Utils::dt);

        
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].update(spaceship, Utils::dt);
            for (int j = 0; j < enemies.size(); j++) {
                if (i != j && Utils::distance(enemies[i].getPosition(), enemies[j].getPosition()) < enemies[i].getSize().x * 1.f) {
                    sf::Vector2f force = enemies[i].evade(enemies[j].getPosition(), enemies[j].getVelocity(), 1);
                    enemies[i].addForce(force);
                }
            }
            sf::Vector2f pixelPos(
                window.mapCoordsToPixel(enemies[i].getPosition()).x,
                window.mapCoordsToPixel(enemies[i].getPosition()).y);

            bool moved = false;
            if (pixelPos.x < -midScreen.x) {
                enemies[i].setPosition(sf::Vector2f(
                    window.mapPixelToCoords(sf::Vector2i(midScreen.x * 3, 0.f)).x,
                    enemies[i].getPosition().y
                ));
                moved = true;
            }
            if (pixelPos.x > midScreen.x * 3) {
                enemies[i].setPosition(sf::Vector2f(
                    window.mapPixelToCoords(sf::Vector2i(-midScreen.x, 0.f)).x,
                    enemies[i].getPosition().y
                ));
                moved = true;
            }
            if (pixelPos.y < -midScreen.y) {
                enemies[i].setPosition(sf::Vector2f(
                    enemies[i].getPosition().x,
                    window.mapPixelToCoords(sf::Vector2i(0.f, midScreen.y * 3)).y
                ));
                moved = true;
            }
            if (pixelPos.y > midScreen.y * 3) {
                enemies[i].setPosition(sf::Vector2f(
                    enemies[i].getPosition().x,
                    window.mapPixelToCoords(sf::Vector2i(0.f, -midScreen.y)).y
                ));
                moved = true;
            }

            if (moved) {
                float angle = atan2(spaceship.getPosition().y - enemies[i].getPosition().y, spaceship.getPosition().x - enemies[i].getPosition().x);

                float mag = Utils::getMagnitude(enemies[i].getVelocity());
                sf::Vector2f newVel(sin(angle), cos(angle));
                newVel = Utils::setMagnitude(newVel, mag);
                enemies[i].setVelocity(newVel);
            }
        }

        background.update(spaceship.getPosition());

        camera.follow(spaceship.getPosition(), spaceship.getVelocity(), Utils::dt);
        //camera.setCenter(spaceship.getPosition());

        window.setView(camera.camera);
        window.clear();

        background.display(window);

        for (Enemy &e : enemies) {
            e.display(window);
        }
        
        spaceship.display(window);
        window.display();
    }

    return 0;
}


void handleResize(sf::RenderWindow& window, Camera2D& camera, unsigned int width, unsigned int height) {
    float windowRatio = (float)width / height;
    float viewRatio = 1200.f / 800.f;
    float sizeX = 1200.f;
    float sizeY = 800.f;

    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

    if (windowRatio > viewRatio) {
        // finestra troppo larga, bande nere a lato
        float newWidth = viewRatio / windowRatio;
        viewport.left = (1.f - newWidth) / 2.f;
        viewport.width = newWidth;
    }
    else if (windowRatio < viewRatio) {
        // finestra troppo alta, bande nere sopra e sotto
        float newHeight = windowRatio / viewRatio;
        viewport.top = (1.f - newHeight) / 2.f;
        viewport.height = newHeight;
    }

    camera.camera.setViewport(viewport);
    window.setView(camera.camera);
}