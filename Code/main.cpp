#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include "arial.h"

enum GameState
{
    Playing, 
    Main_Menu, 
    Lobby, 
    Upgrades
};

class Bullet {
public:
    sf::RectangleShape shape;
    float speed = 800.f;

    Bullet(sf::Vector2f position) {
        shape.setSize({5.f, 15.f});
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
    }

    void update(float deltaTime) {
        shape.move({ 0.f, -speed * deltaTime }); 
    }
};

class Enemy {
public:
    sf::RectangleShape shape;
    
    
    float speed = 75.f;

    Enemy(sf::Vector2f position) {
        shape.setSize({ 50.f, 50.f });
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(position);
    }

    void update(float deltaTime, float stage) {
        shape.move({ 0.f, speed * deltaTime * stage}); 
    }

    
};

int main() {
    // Window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Arcade Shooter", sf::Style::Default & ~sf::Style::Resize);
    window.setFramerateLimit(60);
    

    
    sf::Font font;

    if (!font.openFromMemory(static_cast<const void*>(font_data), static_cast<std::size_t>(moje_pismo_size))) {
        return -1;
    }
    
    
    int shootUpgrade = 0;
    int speedUpgrade = 0;

    // File opening
    int wins = 0;
    {
        std::ifstream in("myfile.txt");
        std::string Myfilestring;
        if (in && std::getline(in, Myfilestring) && !Myfilestring.empty()) {
            try {
                wins = std::stoi(Myfilestring);
            }
            catch (...) {
                wins = 0;
            }
        }
        in.close();
    }

    sf::Text Wins(font);
    Wins.setPosition({ 0,0 });
    Wins.setString("Wins: " + std::to_string(wins));
    Wins.setFillColor(sf::Color::Yellow);

    float enemyStage = 1;

    

    sf::Texture texture("Ply.png");
    sf::Texture textureEnemy("EnemyImg.png");
    sf::Sprite enemySprite(textureEnemy);

    sf::Sprite player(texture);

    sf::SoundBuffer buffer("shoot.wav");
    sf::Sound sound(buffer);

    sf::SoundBuffer buffer2("boom.wav");
    sf::Sound sound2(buffer2);

    sf::SoundBuffer buffer3("victory.wav");
    sf::Sound sound3(buffer3);

    sf::SoundBuffer buffer4("lose.wav");
    sf::Sound sound4(buffer4);

    bool played = false;

    GameState state = GameState::Playing;

    // ===== Upgrades =====

    


    sf::RectangleShape UpgradeButton;
    UpgradeButton.setPosition({5, 275});
    UpgradeButton.setSize({145, 50});
    UpgradeButton.setFillColor(sf::Color(50, 50, 50, 200));
    UpgradeButton.setOutlineColor(sf::Color::White);
    UpgradeButton.setOutlineThickness(5);

    sf::Text updateText(font, "Upgrades", 30);
    updateText.setPosition({10, 280});
    updateText.setFillColor(sf::Color::White);

    // ===== End of upgrades =====



    // ===== Upgrades =====

    sf::RectangleShape UpgradeBackground(sf::Vector2f(800.f, 600.f));
    UpgradeBackground.setFillColor(sf::Color(127, 255, 212));

    sf::Text UpgradesText(font, "Upgrades", 50);
    UpgradesText.setPosition({ 300, 0 });
    UpgradesText.setFillColor(sf::Color::White);
    UpgradesText.setOutlineThickness(3);

    sf::RectangleShape Shootupdate;
    Shootupdate.setPosition({ 300, 200 });
    Shootupdate.setSize({ 200, 50 });
    Shootupdate.setFillColor(sf::Color(50, 50, 50, 200));
    Shootupdate.setOutlineColor(sf::Color::White);
    Shootupdate.setOutlineThickness(5);

    sf::Text ShootUpgradeText(font, "Shoot upgrade:", 30);
    ShootUpgradeText.setPosition({ 80, 205 });
    ShootUpgradeText.setFillColor(sf::Color::Black);
    sf::Text ShootUpgradeText2(font, "5 points", 30);
    ShootUpgradeText2.setPosition({ 330, 205 });
    ShootUpgradeText2.setFillColor(sf::Color::White);

    sf::RectangleShape Speedupgrade;
    Speedupgrade.setPosition({ 300, 400 });
    Speedupgrade.setSize({ 200, 50 });
    Speedupgrade.setFillColor(sf::Color(50, 50, 50, 200));
    Speedupgrade.setOutlineColor(sf::Color::White);
    Speedupgrade.setOutlineThickness(5);

    sf::Text SpeedUpgradeText(font, "Speed upgrade:", 30);
    SpeedUpgradeText.setPosition({ 80, 405 });
    SpeedUpgradeText.setFillColor(sf::Color::Black);
    sf::Text SpeedUpgradeText2(font, "5 points", 30);
    SpeedUpgradeText2.setPosition({ 330, 405 });
    SpeedUpgradeText2.setFillColor(sf::Color::White);

    sf::RectangleShape RedX;
    RedX.setPosition({ 25,25 });
    RedX.setSize({ 50,50 });
    RedX.setFillColor(sf::Color::Red);

    sf::Text RedXText(font, "X", 50);
    RedXText.setPosition({ 33, 19 });
    RedXText.setOutlineThickness(2);

    // ===== End of upgrades =====



    // ===== Main Menu =====

    sf::RectangleShape background(sf::Vector2f(800.f, 600.f));
    background.setFillColor(sf::Color(0, 0, 0, 200));

    sf::RectangleShape Resume;
    Resume.setPosition({ 300.f, 275.f });
    Resume.setSize({ 200.f, 50.f });
    // Make the resume button visible: slightly lighter fill and white outline
    Resume.setFillColor(sf::Color(50, 50, 50, 200));
    Resume.setOutlineColor(sf::Color::White);
    Resume.setOutlineThickness(5);

    sf::Text resumeT(font, "Resume", 30);
    resumeT.setPosition({ 340.f, 280.f });
    
    resumeT.setFillColor(sf::Color::White);
    // ===== End of Main Menu =====


    
    bool gameOver = false;
    
    player.setPosition({ 400.f, 500.f });
    float playerSpeed = 400.f;

    int score = 0;

    

    sf::Text scoreText(font, "Score: 0", 30);
    scoreText.setPosition({670.f, 0.f});

    sf::Text gameOverText(font, "Game Over - you Lose!", 30);

    sf::FloatRect textBounds = gameOverText.getLocalBounds();

    gameOverText.setOrigin({
        textBounds.position.x + textBounds.size.x/2,
        textBounds.position.y + textBounds.size.y/2
    });
    sf::Vector2u windowSize = window.getSize();
    gameOverText.setPosition({
        static_cast<float>(windowSize.x) / 2.0f,
        static_cast<float>(windowSize.y) / 2.0f
        });
    
    gameOverText.setFillColor(sf::Color::Red);
    
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> randomX(0.f, 750.f);

    
    sf::Clock clock;
    sf::Clock shootClock;
    sf::Clock enemyClock;
    sf::Clock buttonClock;
    float shootCooldown = 0.2f; 
    float enemySpawn = 2.f;
    float buttonCooldown = 0.2f;

    // ==== The main loop ====
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        
        if (state != GameState::Main_Menu && state != GameState::Upgrades) {
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) && player.getPosition().x > 0) {
                
                player.move({ - playerSpeed * deltaTime, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) && player.getPosition().x < 800 - 50) {
                player.move({ playerSpeed * deltaTime, 0.f });
            }

            // Shooting
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
                sf::Vector2f bulletPos = player.getPosition() + sf::Vector2f(22.f, 0.f);
                bullets.push_back(Bullet(bulletPos));
                sound.play();
                shootClock.restart();
            }
            for (size_t i = 0; i < enemies.size(); ) {
                enemies[i].update(deltaTime, enemyStage);

                i++;
            }
            if (enemyClock.getElapsedTime().asSeconds() >= enemySpawn) {
                enemies.push_back(Enemy({ randomX(gen), 0 }));
                enemyClock.restart();
            }
            for (size_t i = 0; i < enemies.size(); ) {
                enemies[i].update(deltaTime, enemyStage);

                if (enemies[i].shape.getPosition().y > 600) {
                    enemies.erase(enemies.begin() + i); 
                    score -= 3;
                }
                else {
                    i++;
                }
            }
        }
        if (state == GameState::Main_Menu) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                if (Resume.getGlobalBounds().contains({ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) })) {
                    state = GameState::Playing;
                }
            }
        }
        
        if (state == GameState::Playing) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                if (UpgradeButton.getGlobalBounds().contains({static_cast<float>(mousepos.x), static_cast<float>(mousepos.y)})) {
                    state = GameState::Upgrades;
                    enemyClock.stop();
                    shootClock.stop();
                    
                }
            }
        }

        if (state == GameState::Upgrades) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);

                if (RedX.getGlobalBounds().contains({ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) })) {
                    state = GameState::Playing;
                    enemyClock.start();
                    shootClock.start();

                }else if (Shootupdate.getGlobalBounds().contains({ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) }) && buttonClock.getElapsedTime().asSeconds() >= buttonCooldown) {
                    
                    switch (shootUpgrade) {
                    case 0:
                        if (score >= 5) {
                            shootUpgrade++;
                            ShootUpgradeText2.setString("10 points");
                            score -= 5;
                            shootUpgrade += 1;
                            shootCooldown = 0.16f;
                        }
                        
                        break;
                    case 1:
                        if (score >= 10) {
                            shootUpgrade++;
                            ShootUpgradeText2.setString("15 points");
                            score -= 10;
                            shootUpgrade += 1;
                            shootCooldown = 0.13f;
                        }
                        
                        break;
                    case 2:
                        if (score >= 15) {
                            ShootUpgradeText2.setString("Max level");
                            score -= 15;
                            shootUpgrade += 1;
                            shootCooldown = 0.11f;
                        }
                        
                        break;
                    }
                    
                    buttonClock.restart();
                    

                }else if (Speedupgrade.getGlobalBounds().contains({ static_cast<float>(mousepos.x), static_cast<float>(mousepos.y) }) && buttonClock.getElapsedTime().asSeconds() >= buttonCooldown) {
                    
                    switch (speedUpgrade) {
                    case 0:
                        if (score >= 5) {
                            speedUpgrade++;
                            SpeedUpgradeText2.setString("10 points");
                            score -= 5;
                            speedUpgrade += 1;
                            playerSpeed = 440.f;
                        }
                        
                        break;
                    case 1:
                        if (score >= 10) {
                            speedUpgrade++;
                            SpeedUpgradeText2.setString("15 points");
                            score -= 10;
                            speedUpgrade += 1;
                            playerSpeed = 470.f;
                        }
                        
                        break;
                    case 2:
                        if (score >= 15) {
                            SpeedUpgradeText2.setString("Max level");
                            score -= 15;
                            speedUpgrade += 1;
                            playerSpeed = 490.f;
                        }
                        
                    }
                    buttonClock.restart();

                }
            }
        }
        

        if (state == GameState::Playing) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape) && buttonClock.getElapsedTime().asSeconds() >= buttonCooldown) {
                state = GameState::Main_Menu;
                buttonClock.restart();
            }

        }
        else if (state == GameState::Main_Menu) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Escape) && buttonClock.getElapsedTime().asSeconds() >= buttonCooldown) {
                state = GameState::Playing;
                buttonClock.restart();
            }

        }

        // Erase bullet
        
        for (size_t i = 0; i < bullets.size(); ) {
            bullets[i].update(deltaTime);

            if (bullets[i].shape.getPosition().y < 0) {
                bullets.erase(bullets.begin() + i);
            }
            else {
                i++; 
            }
        }
        

        for (size_t i = 0; i < enemies.size(); ) {
            bool enemyDestroyed = false;

            for (size_t j = 0; j < bullets.size(); ) {
                sf::FloatRect boundsEnemy = enemies[i].shape.getGlobalBounds();
                sf::FloatRect boundsBullet = bullets[j].shape.getGlobalBounds();

                boundsEnemy.size.x -= 20;
                boundsEnemy.size.y -= 30;

                boundsEnemy.position.x += 10;
                boundsEnemy.position.y += 15;

                if (boundsEnemy.findIntersection(boundsBullet)) {
                    enemies.erase(enemies.begin() + i);
                    bullets.erase(bullets.begin() + j);
                    enemyDestroyed = true;
                    score += 1;
                    if (score >= 50 && enemyStage != 1.5) {
                        enemyStage = 1.5;
                    }
                    sound2.play();
                    break; 
                }
                else {
                    j++; 
                }
            }

            if (!enemyDestroyed) {
                i++; 
            }
        }
        
        if (score >= 100) {
            clock.stop();
            enemyClock.stop();
            shootClock.stop();
            gameOver = true;
            gameOverText.setFillColor(sf::Color::Green);
            gameOverText.setString("Game Over - You Win!");
            if (!played) {
                sound3.play();
                played = true;
                // ==== File saving ====
                wins += 1;
                Wins.setString("Wins: " + std::to_string(wins));
                std::ofstream out("myfile.txt", std::ios::trunc);
                if (out) {
                    out << wins;
                    out.close();
                }
            }
            
            
        }
        else if (score <= -50) {
            clock.stop();
            enemyClock.stop();
            shootClock.stop();
            gameOver = true;
            gameOverText.setFillColor(sf::Color::Red);
            if (!played) {
                sound4.play();
                played = true;
            }
            
        }

        scoreText.setString("Score: " + std::to_string(score));
        

        // ==== Drawing ====
        window.clear();

        if (gameOver) {
            window.draw(gameOverText);
        }
        
        if (state != GameState::Upgrades) {
            window.draw(UpgradeButton);
            window.draw(updateText);
            window.draw(player);
        
            window.draw(scoreText);

            window.draw(Wins);
            for (const auto& bullet : bullets) {
                window.draw(bullet.shape);
            }
            for (const auto& enemy : enemies) {
                enemySprite.setPosition(enemy.shape.getPosition());
                window.draw(enemySprite);
            }
        }

        
        if (state == GameState::Upgrades) {
            window.draw(UpgradeBackground);
            window.draw(UpgradesText);
            window.draw(Shootupdate);
            window.draw(Speedupgrade);
            window.draw(ShootUpgradeText);
            window.draw(ShootUpgradeText2);
            window.draw(SpeedUpgradeText);
            window.draw(SpeedUpgradeText2);
            window.draw(RedX);
            window.draw(RedXText);
        }
        
        // Main menu
        
        if (state == GameState::Main_Menu) {
            window.draw(background);
            window.draw(Resume);
            window.draw(resumeT);
        }


        window.display();
    }

    return 0;
}