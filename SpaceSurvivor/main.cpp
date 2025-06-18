#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Utils.h"
#include "SpaceShip.h"
#include "Camera2D.h"
#include "Background.h"
#include "Enemy.h"
#include "Bullet.h"

#include "imgui.h"
#include "imgui-SFML.h"

//with this you can resize the window, all made by chatGPT in fact it doesnt work right
void handleResize(sf::RenderWindow& window, Camera2D& camera, unsigned int width, unsigned int height);
//function that moves enemy out of bounds
void teleportEnemyOutOfBounds(std::unique_ptr<Enemy>& enemy, SpaceShip& spaceship, sf::RenderWindow& window, sf::Vector2f screenSize);
//simple check collision
bool checkBulletCollision(Bullet& bullet, sf::Vector2f position, sf::Vector2f size);

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    ImGui::GetIO().FontGlobalScale = 1.3f;

    sf::Vector2f midScreen(window.getSize().x / 2.f, window.getSize().y / 2.f);
    sf::Vector2f screenSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
    Camera2D camera(sf::Vector2f(screenSize.x * 0.9, screenSize.y * 0.9), 10);
    
    //resource loading
    sf::Sound song;
    sf::SoundBuffer song1;
    song1.loadFromFile("./Audio/game.wav");

    song.setBuffer(song1);
    song.setLoop(true);
    
    sf::Texture bgTexture;
    bgTexture.loadFromFile("./Sprites/background3.png");

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("./Sprites/bullet1.png");
    sf::Texture bulletTexture2;
    bulletTexture2.loadFromFile("./Sprites/bullet2.png");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("./Sprites/player.png");

    //initialize class
    SpaceShip spaceship(midScreen, playerTexture, bulletTexture);
    Background background(bgTexture, spaceship.getPosition());

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("./Sprites/enemy.png");


    //after hours of problem due to normale vector i decided with the help of chatGPT
    //to use unique_ptr, because otherwise each time i would change the array, so add/remove an element
    //some enemies would lose their texture reference due to their change of address in memory, with unique_ptr 
    //it doesn't happen anymore so that why i'm using it
    std::vector<std::unique_ptr<Enemy>> enemies;

    int nEnemies = 10;
    float incr = Utils::degreesToRadians(360.f / nEnemies);
    float angle = 0;
    for (int i = 0; i < nEnemies; i++) {
        float x = spaceship.getPosition().x + 700 * cos(angle);
        float y = spaceship.getPosition().y + 700 * sin(angle);
      
        auto enemyPtr = std::make_unique<Enemy>(sf::Vector2f(x, y), true, enemyTexture, bulletTexture2);

        enemyPtr->setVelocity(Utils::normalize(enemyPtr->getPosition() - spaceship.getPosition()));

        enemies.push_back(std::move(enemyPtr));
        angle += incr;
    }

    //key/mouse movement mode
    bool movementMode = false;
    //time alive
    float timeElapsedSeconds = 0.f;
    //max time alive this session/life
    float maxTimeElapsedSeconds = 0.f;
    //spawn cooldown of enemies
    float spawnCooldown = 2.5f;

    //variable that control the game, if paused or not
    bool gameOn = false;
    bool lastGameOn = false;

    //a record of the longest time survive
    float maxSurviveTime = 0.f;
    sf::Clock timeSurviving;

    while (window.isOpen()){
    
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

       
        if (gameOn) {
            if (gameOn != lastGameOn) {
                timeSurviving.restart();
            }

            timeElapsedSeconds += Utils::dt;

            #pragma region spawnEnemy logic
            if (timeElapsedSeconds > spawnCooldown) {
                maxTimeElapsedSeconds += timeElapsedSeconds;
                //change dynamically the spawncooldown
                spawnCooldown = Utils::map(maxTimeElapsedSeconds, 0.f, 120.f, 2.5f, 0.7f);
                timeElapsedSeconds = 0;
                
                //yes i could do a functio for the spawn, but im lazy as hell
                angle = Utils::randomFloat(0.f, 3.14f);
                float x = spaceship.getPosition().x + 700 * cos(angle);
                float y = spaceship.getPosition().y + 700 * sin(angle);
                
                auto enemyPtr = std::make_unique<Enemy>(sf::Vector2f(x, y),false, enemyTexture, bulletTexture2);

                enemyPtr->setVelocity(Utils::normalize(enemyPtr->getPosition() - spaceship.getPosition()));

                enemies.push_back(std::move(enemyPtr));
            }
            #pragma endregion   

            #pragma region Enemies logic

                    for (int i = 0; i < enemies.size(); i++) {

                        enemies[i]->update(spaceship, Utils::dt);

                        //for each enemy it has to avoid others enemy (just visually)
                        for (int j = 0; j < enemies.size(); j++) {
                            if (i != j && Utils::distance(enemies[i]->getPosition(), enemies[j]->getPosition()) < enemies[i]->getSize().x * 1.f) {
                                sf::Vector2f force = enemies[i]->evade(enemies[j]->getPosition(), enemies[j]->getVelocity(), 1);
                                enemies[i]->addForce(force);
                            }

                            //enemy bullets collide with spaceship
                            std::vector<Bullet>& eBullets = enemies[i]->getBullets();
                            for (Bullet& b : eBullets) {
                                if (b.toRemove == false && checkBulletCollision(b, spaceship.getPosition(), spaceship.getSize())){
                                    b.toRemove = true;
                                    spaceship.damage(5);
                                }
                            }
                        }

                        teleportEnemyOutOfBounds(enemies[i], spaceship, window, screenSize);

                        std::vector<Bullet>& bullets = spaceship.getBullets();

                        //for each bullet of the spaceship it check if collide with each enemy
                        for (Bullet& b : bullets) {
                            if (b.toRemove == false && checkBulletCollision(b, enemies[i]->getPosition(), enemies[i]->getSize())){
                                b.toRemove = true;
                                enemies[i]->toRemove = true;
                            }
                        }

                    }

                    for (int i = enemies.size() - 1; i >= 0; i--) {
                        if (enemies[i]->toRemove) {
                            enemies[i]->deleteSelf();
                            enemies.erase(enemies.begin() + i);
                        }
                    }
            #pragma endregion

            #pragma region Spaceship logic

            sf::Vector2f mousePos(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y));

            spaceship.setMode(movementMode);

            spaceship.update(
                mousePos,
                sf::Vector2f(
                    static_cast<float>(window.mapCoordsToPixel(spaceship.getPosition()).x),
                    static_cast<float>(window.mapCoordsToPixel(spaceship.getPosition()).y)),
                Utils::dt);

            //if that reset the game, i COULD HAVE DONE A FUNCTION, but again im lazy, i will do it for the intake
            if (spaceship.getHp() <= 0) {
                gameOn = false;
                
                spaceship.setHp(100.f);
                spaceship.setPosition(midScreen);
                spaceship.setVelocity(sf::Vector2f(0, 0));

                enemies.clear();
                
                int nEnemies = 10;
                float incr = Utils::degreesToRadians(360.f / nEnemies);
                float angle = 0;
                
                for (int i = 0; i < nEnemies; i++) {
                    float x = spaceship.getPosition().x + 700 * cos(angle);
                    float y = spaceship.getPosition().y + 700 * sin(angle);
                    
                    auto enemyPtr = std::make_unique<Enemy>(sf::Vector2f(x, y), true, enemyTexture, bulletTexture2);

                    enemyPtr->setVelocity(Utils::normalize(enemyPtr->getPosition() - spaceship.getPosition()));

                    enemies.push_back(std::move(enemyPtr));
                    angle += incr;
                }

                camera.setCenter(spaceship.getPosition());
                if (song.getStatus() == sf::Sound::Playing) {
                    song.play();
                }

                if (timeSurviving.getElapsedTime().asSeconds() > maxSurviveTime) {
                    maxSurviveTime = timeSurviving.getElapsedTime().asSeconds();
                }
                
            }
    #pragma endregion


            camera.follow(spaceship.getPosition(), spaceship.getVelocity(), Utils::dt);

            lastGameOn = gameOn;
            
        }

            
        background.update(spaceship.getPosition());

        ImGui::SFML::Update(window, sf::seconds(Utils::dt));

        //IMGUI gui
        ImGui::Begin("Game Menu");
        ImGui::Text("Avoid, Kill and Survive as long as you can!");
        ImGui::Text("\"Space\" to shoot");
        ImGui::Text("\"w\" or LeftClick to move");
        ImGui::Checkbox("Start Game", &gameOn);
        ImGui::Text("Choose to move with \"w\" or with LeftClick: ");
        ImGui::Checkbox("Keyboard mode", &movementMode);
        ImGui::Text("Time survived: %.2f", gameOn ? timeSurviving.getElapsedTime().asSeconds() : 0.f);
        ImGui::Text("Best time survived: %.2f", maxSurviveTime);
        
        if(ImGui::Button("Toggle Music")) {
            if (song.getStatus() == sf::Sound::Playing) {
                song.stop();
            }
            else {
                song.play();
            }

        }

        //---------------------DISPLAY---------------------------
        window.setView(camera.camera);
        window.clear();

        background.display(window);

        for (auto& e : enemies) {
            e->display(window);
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

void teleportEnemyOutOfBounds(std::unique_ptr<Enemy>& enemy, SpaceShip& spaceship, sf::RenderWindow& window, sf::Vector2f screenSize) {
    //madly in love with: window.mapCoordsToPixel and window.mapPixelToCoords
    //it makes everythings a lot easier
    
    sf::Vector2f pixelPos = static_cast<sf::Vector2f>(window.mapCoordsToPixel(enemy->getPosition()));
    sf::Vector2f newPos = enemy->getPosition();
    bool moved = false;

    //if out of bounds it moves that coordinate at the other bound
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
        enemy->setPosition(newPos);

        // rotate the enemy towards the spaceship
        sf::Vector2f toPlayer = spaceship.getPosition() - newPos;
        sf::Vector2f newVel = Utils::setMagnitude(Utils::normalize(toPlayer), Utils::getMagnitude(enemy->getVelocity()));
        enemy->setVelocity(newVel);
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