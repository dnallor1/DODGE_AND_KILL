#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>

int main()
{
    srand(time(NULL));
    float move_rocket = 0.8;
    float move_background = 0.02;
    float scale;
    int kill = 0;
    float score = 0;
    //    int fiu = 0;

    sf::SoundBuffer buffer, screen, collide;
    buffer.loadFromFile("bonus.wav");
    screen.loadFromFile("intro.wav");
    collide.loadFromFile("collide.wav");

    sf::Sound sound_buffer, sound_screen, sound_collide;
    sound_buffer.setBuffer(buffer);
    sound_screen.setBuffer(screen);
    sound_collide.setBuffer(collide);

    sf::Texture texture, texture_rocket, texture_rock, texture_heart, texture_diamond, texture_gameover;
    texture.loadFromFile("background.png");
    texture_rocket.loadFromFile("ship.png");
    texture_rock.loadFromFile("rocks.png");
    texture_diamond.loadFromFile("diamond.png");
    texture_gameover.loadFromFile("gameover.png");
    texture_heart.loadFromFile("heart.png");

    std::vector<sf::Sprite> hearts;
    sf::Sprite heart;
    heart.scale(0.06, 0.06);
    heart.setTexture(texture_heart);

    heart.setPosition(sf::Vector2f(850, 35));
    hearts.push_back(heart);
    heart.setPosition(sf::Vector2f(880, 35));
    hearts.push_back(heart);
    heart.setPosition(sf::Vector2f(910, 35));
    hearts.push_back(heart);
    heart.setPosition(sf::Vector2f(940, 35));
    hearts.push_back(heart);

    sf::Sprite gameover;
    gameover.setTexture(texture_gameover);
    gameover.setScale(2.8, 2.6);

    sf::Sprite rocket, background;
    rocket.setTexture(texture_rocket);
    rocket.setScale(0.06, 0.06);
    sf::Vector2f rocket_position(440, 750);

    sf::Vector2f background_position(0, -450);
    background.setPosition(background_position);
    background.setTexture(texture);
    background.setScale(sf::Vector2f(1.27, 1.16));

    sf::Sprite rock[800];
    for (int i = 0; i < 800; i++) {
        scale = (rand() % 14 + 4) * 0.006;
        rock[i].setTexture(texture_rock);
        rock[i].setScale(scale, scale);
        rock[i].setPosition(sf::Vector2f(rand() % 1000, rand() % 9000000 * (-1)));
    }
    sf::Sprite diamond[25];
    for (int i = 0; i < 25; i++) {
        diamond[i].setTexture(texture_diamond);
        diamond[i].setScale(sf::Vector2f(0.1, 0.1));
        diamond[i].setPosition(sf::Vector2f(rand() % 1000, rand() % 9000000 * (-1)));
    }

    sf::Font font;
    font.loadFromFile("fn.otf");

    sf::Text text, score_, point_, points, scores;
    text.setFont(font);
    text.setString("WELCOME. LET'S RIDE!!! press Enter");
    text.setCharacterSize(50);
    text.setPosition(50, 700);
    text.setFillColor(sf::Color::Red);

    score_.setFont(font);
    score_.setFillColor(sf::Color::Red);
    score_.setCharacterSize(50);
    score_.setString("SCORE:");
    score_.setPosition(180, 820);

    point_.setFont(font);
    point_.setCharacterSize(50);
    point_.setFillColor(sf::Color::Blue);
    point_.setPosition(480, 820);

    points.setFont(font);
    points.setCharacterSize(20);
    points.setFillColor(sf::Color::Blue);
    points.setPosition(800, 10);
    points.setString("POINTS:");

    scores.setFont(font);
    scores.setCharacterSize(20);
    scores.setFillColor(sf::Color::Red);
    scores.setPosition(900, 10);

    sf::RectangleShape hitbox_rocket;
    hitbox_rocket.setSize(sf::Vector2f(20, 60));
    sf::Vector2f hitbox_rocket_position(rocket_position.x + 30, rocket_position.y + 10);

    sf::RenderWindow window(sf::VideoMode(1000, 900), "Crazy Rocket");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && rocket_position.x > 0) {
            rocket_position.x = rocket_position.x - move_rocket;
            hitbox_rocket_position.x = hitbox_rocket_position.x - move_rocket;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && rocket_position.x < 925) {
            rocket_position.x = rocket_position.x + move_rocket;
            hitbox_rocket_position.x = hitbox_rocket_position.x + move_rocket;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && rocket_position.y > 0) {
            rocket_position.y = rocket_position.y - move_rocket;
            hitbox_rocket_position.y = hitbox_rocket_position.y - move_rocket;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && rocket_position.y < 825)  {
            rocket_position.y = rocket_position.y + move_rocket;
            hitbox_rocket_position.y = hitbox_rocket_position.y + move_rocket;
        }
        background_position.y = background_position.y + move_background;
        rocket.setPosition(rocket_position);
        hitbox_rocket.setPosition(hitbox_rocket_position);
        background.setPosition(background_position);
        window.clear(sf::Color::Black);
        if (kill == 0)  {
            sound_screen.play();
            window.draw(background);
            window.draw(rocket);
            window.draw(text);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && kill < 2) kill = 1;
        for (int i = 0; i < 800; i++)  {
            rock[i].getGlobalBounds();
            if (rock[i].getGlobalBounds().intersects(hitbox_rocket.getGlobalBounds())) kill = 2;
        }
        if (kill == 1) {
            for (int i = 0; i < 25; i++) {
                diamond[i].getGlobalBounds();
                if (diamond[i].getGlobalBounds().intersects(hitbox_rocket.getGlobalBounds())) {
                    score = score + 100;
                    diamond[i].scale(0.0001, 0.0001);
                    sound_buffer.play();
                    break;
                }
            }
            score = score + 0.01;
            window.draw(background);
            window.draw(rocket);
            //            gameover.play();
            for (int k = 0; k < 800; k++) {
                sf::Vector2f rock_position(rock[k].getPosition());
                rock[k].setPosition(rock_position.x, rock_position.y + 0.8);
                window.draw(rock[k]);
            }
            for (int k = 0; k < 25; k++) {
                sf::Vector2f diamond_position(diamond[k].getPosition());
                diamond[k].setPosition(diamond_position.x, diamond_position.y + 0.8);
                window.draw(diamond[k]);
            }
            for(auto &heart_ : hearts) {
                window.draw(heart_);
            }
            window.draw(points);
            scores.setString(std::to_string(score));
            window.draw(scores);
        }
        if (kill == 2) {
            window.draw(background);
            window.draw(gameover);
            window.draw(score_);
            point_.setString(std::to_string(score));
            window.draw(point_);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && kill == 2) {
            while (true) {
                window.close();
            }
        }
        window.display();
    }
}
