#include <iostream>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "SpaceShip.h"
#include "Camera2D.h"
#include "Background.h"
#include "Enemy.h"
#include "Bullet.h"

#include "imgui.h"
#include "imgui-SFML.h"

void handleResize(sf::RenderWindow& window, Camera2D& camera, unsigned int width, unsigned int height);
void teleportEnemyOutOfBounds(Enemy& enemy, SpaceShip& spaceship, sf::RenderWindow& window, sf::Vector2f screenSize);
bool checkBulletCollision(Bullet& bullet, sf::Vector2f position, sf::Vector2f size);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920,1080), "SFML works!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    //ImGui::SFML::Init(window);

    Camera2D camera(sf::Vector2f(1920, 1080), 10);
    sf::Vector2f midScreen(window.getSize().x / 2.f, window.getSize().y / 2.f);
    sf::Vector2f screenSize(window.getSize().x, window.getSize().y);

    sf::Texture bgTexture;
    bgTexture.loadFromFile("./Sprites/background2.png");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("./Sprites/bullet1.png");
    sf::Texture bulletTexture2;
    bulletTexture2.loadFromFile("./Sprites/bullet2.png");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("./Sprites/player.png");


    SpaceShip spaceship(sf::Vector2f(960, 540), midScreen, playerTexture, bulletTexture);
    Background background(bgTexture, spaceship.getPosition());
    
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("./Sprites/enemy.png");

    std::vector<Enemy> enemies;
    int nEnemies = 30;
    float incr = Utils::degreesToRadians(360.f / nEnemies);
    float angle = 0;
    for (int i = 0; i < nEnemies; i++) {
        float x = spaceship.getPosition().x + 700 * cos(angle);
        float y = spaceship.getPosition().y + 700 * sin(angle);
        enemies.push_back(Enemy(sf::Vector2f(x,y), enemyTexture, bulletTexture2));
        enemies[i].setVelocity(Utils::normalize(enemies[i].getPosition() - spaceship.getPosition()));
        angle += incr;
    }
    

    while (window.isOpen())
    {
        Utils::updateDt();

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

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
            std::vector<Bullet>& bullets = spaceship.getBullets();

            for (Bullet& b : bullets) {
                if (b.toRemove == false && checkBulletCollision(b, enemies[i].getPosition(), enemies[i].getSize())) {
                    std::cout << "hit" << std::endl;
                    b.toRemove = true;
                    enemies[i].toRemove = true;
                }
            }

            enemies[i].update(spaceship, Utils::dt);
            for (int j = 0; j < enemies.size(); j++) {
                if (i != j && Utils::distance(enemies[i].getPosition(), enemies[j].getPosition()) < enemies[i].getSize().x * 1.f) {
                    sf::Vector2f force = enemies[i].evade(enemies[j].getPosition(), enemies[j].getVelocity(), 1);
                    enemies[i].addForce(force);
                }
            }

            teleportEnemyOutOfBounds(enemies[i], spaceship, window, screenSize);
            
        }

        for (int i = enemies.size() - 1; i >= 0; i--) {
            if (enemies[i].toRemove) {
                enemies[i].deleteBullet();
                enemies.erase(enemies.begin() + i);
            }
        }

        background.update(spaceship.getPosition());

        camera.follow(spaceship.getPosition(), spaceship.getVelocity(), Utils::dt);
        //camera.setCenter(spaceship.getPosition());

        ImGui::SFML::Update(window, sf::seconds(Utils::dt));

        ImGui::Begin("Debug");
        ImGui::Text("DeltaTime: %.3f", Utils::dt);

        window.setView(camera.camera);
        window.clear();

        background.display(window);

        for (Enemy &e : enemies) {
            e.display(window);
        }
        
        spaceship.display(window);


        ImGui::End();
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}

bool checkBulletCollision(Bullet& bullet, sf::Vector2f position, sf::Vector2f size) {
    sf::Vector2f bPos = bullet.getPosition();

    if (bPos.x < position.x + size.x / 2.f &&
        bPos.x > position.x - size.x / 2.f &&
        bPos.y < position.y + size.y / 2.f &&
        bPos.y > position.y - size.y / 2.f) {
        return true;
    }
    
    return false;
}

void teleportEnemyOutOfBounds(Enemy& enemy, SpaceShip& spaceship,sf::RenderWindow &window, sf::Vector2f screenSize) {
    sf::Vector2f pixelPos = static_cast<sf::Vector2f>(window.mapCoordsToPixel(enemy.getPosition()));
    sf::Vector2f newPos = enemy.getPosition();
    bool moved = false;

    if (pixelPos.x < 0) {
        newPos.x = window.mapPixelToCoords(sf::Vector2i(static_cast<int>(screenSize.x), 0)).x;
        moved = true;
    }
    else if (pixelPos.x > screenSize.x) {
        newPos.x = window.mapPixelToCoords(sf::Vector2i(0, 0)).x;
        moved = true;
    }

    if (pixelPos.y < 0) {
        newPos.y = window.mapPixelToCoords(sf::Vector2i(0, static_cast<int>(screenSize.y))).y;
        moved = true;
    }
    else if (pixelPos.y > screenSize.y) {
        newPos.y = window.mapPixelToCoords(sf::Vector2i(0, 0)).y;
        moved = true;
    }

    if (moved) {
        enemy.setPosition(newPos);

        // Reindirizza verso il player
        sf::Vector2f toPlayer = spaceship.getPosition() - newPos;
        sf::Vector2f newVel = Utils::setMagnitude(Utils::normalize(toPlayer), Utils::getMagnitude(enemy.getVelocity()));
        enemy.setVelocity(newVel);
    }
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