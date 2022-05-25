#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const static auto DEFAULT_COLOR = sf::Color(100, 250, 50);

class CustomRectangleShape : public sf::RectangleShape {
public:
    CustomRectangleShape(const sf::Vector2f &size, const sf::Vector2f &position) : sf::RectangleShape(size) {
        setPosition(position);
    }
    void setSpeed(const int& y_speed, const int & ro_speed) {
        //        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }
    void animate(const sf::Time &elapsed){
        bouncce();
        float dt = elapsed.asSeconds();
        move(x_speed_*dt,y_speed_*dt);
        rotate(ro_speed_*dt);
    }

    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }
private:
    int x_speed_ = 0 ;
    int y_speed_ = 0 ;
    int ro_speed_ = 0 ;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;

    void bouncce(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if(rectangle_bounds.top <= u_bound_){
            y_speed_ = abs(y_speed_);
        }
        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
            y_speed_ = abs(y_speed_);
        }
        if(rectangle_bounds.left <= l_bound_ ){
            x_speed_ = abs(x_speed_);
        }
        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
            x_speed_ = abs(x_speed_) * -1;
        }
    }
};

class AnimatedSprite : public sf::Sprite {
public:
    AnimatedSprite(const int fps, const std::string& path) : fps_(fps)//make an initializer list for fps_
    {
        sf::Texture texture;
        if (!texture_.loadFromFile(path)) {
            std::cerr << "Could not load texture" << path << std::endl;
        }
        setTexture(texture_);
        setTextureRect(sf::IntRect(50, 0, 50, 37));
    }
    void setSpeed(const int& x_speed, const int& y_speed, const int & ro_speed) {
        x_speed_ = x_speed;
        y_speed_ = y_speed;
        ro_speed_ = ro_speed;
    }

    void animate(const sf::Time &elapsed){
        bouncce();
        float dt = elapsed.asSeconds();
        move(x_speed_*dt,y_speed_*dt);
        rotate(ro_speed_*dt);
    }
    void jump(const sf::Time &elapsed) {
        float dt = elapsed.asSeconds();
        int g = -20;
        bouncce();
        move(0,0.5 * g * pow(dt,2));
    }
    void setBounds(const float& l_bound, const float& r_bound,const float& u_bound,const float& d_bound){
        l_bound_  = l_bound  ;
        r_bound_  = r_bound  ;
        u_bound_  = u_bound  ;
        d_bound_  = d_bound  ;
    }
    void add_animation_frame(const sf::IntRect& frame){
        running_frames.emplace_back(frame);
    }
    void moveInDirection(const sf::Time &elapsed, const sf::Keyboard::Key &key, std::vector<sf::Sprite> &hearts)  {
        float dt = elapsed.asSeconds();
        if(key == sf::Keyboard::Up) {
            y_speed_ = -1*abs(y_speed_);
            bouncce();
            hearts.size();
            move(0, y_speed_ * dt);
        }
        else if(key == sf::Keyboard::Down)  {
            y_speed_ = abs(y_speed_);
            bouncce();
            move(0, y_speed_ * dt);
        }
        else if(key == sf::Keyboard::Left)  {
            t_ += dt;
            if(t_ > 1.0/fps_){
                fragments_index++;
                t_ = 0.0;
            }
            if(fragments_index == running_frames.size()){
                fragments_index = 0;
            }
            setTextureRect(running_frames[fragments_index]);
            x_speed_ = -1*abs(x_speed_);
            //            bouncce();
            move(x_speed_ * dt, 0);
        }
        else if(key == sf::Keyboard::Right)  {
            t_ += dt;
            if(t_ > 1.0/fps_){
                fragments_index++;
                t_ = 0.0;
            }
            if(fragments_index == running_frames.size()){
                fragments_index = 0;
            }
            setTextureRect(running_frames[fragments_index]);
            x_speed_ = abs(x_speed_);
            bouncce();
            move(x_speed_ * dt, 0);
        }
    }
private:
    sf::Texture texture_;
    int fps_;
    int x_speed_ = 0 ;
    int y_speed_ = 0 ;
    int ro_speed_ = 0 ;
    float l_bound_ = 0;
    float r_bound_ = 0;
    float u_bound_ = 0;
    float d_bound_ = 0;
    float t_ = 0.0;
    unsigned int fragments_index = 0;
    std::vector<sf::IntRect> running_frames;

    void bouncce(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if(rectangle_bounds.top <= u_bound_){
            y_speed_ = abs(y_speed_);
        }
        if(rectangle_bounds.top + rectangle_bounds.height >= d_bound_){
            y_speed_ = abs(y_speed_) * -1;
        }
        if(rectangle_bounds.left <= l_bound_ ){
            x_speed_ = abs(x_speed_);
        }
        if(rectangle_bounds.left + rectangle_bounds.width >= r_bound_){
            x_speed_ = abs(x_speed_) ;
        }
    }
};
int main() {
    // create the window
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1800, 900), "My window");
    std::srand(std::time(nullptr));
    std::vector<CustomRectangleShape> rectangles;
    for(int i=0; i<50; i++) {
        sf::Vector2f size(25.0, 25.0);
        sf::Vector2f position(std::rand() % window.getSize().x, std::rand() % window.getSize().y);
        rectangles.emplace_back(CustomRectangleShape(size, position));

        for(auto &rec : rectangles) {
            int x = rand() % 1920;
            int y = rand() % 1500 - 1500;
            rec.setFillColor(sf::Color(100, 50, 250));
            rec.setBounds(0, window.getSize().x, 0, window.getSize().y);
            rec.setOrigin(45/2, 45/2);
            rec.setSpeed(std::rand() % 500, rand() % 100);
            rec.setPosition(x, y);
        }
        clock.restart();
    }

    //    //health
    //    sf::Texture texture_health;
    //    if(!texture_health.loadFromFile("health.png")) {
    //        return 1; }
    //    std::vector<sf::Sprite> healths;
    //    sf::Sprite health;
    //    health.scale(0.06, 0.06);
    //    health.setTexture(texture_health);
    //    health.move(0, 20);

    //    //heart_1
    //    health.setPosition(sf::Vector2f(rand() % 1920, rand() % 1500 - 1500));
    //    healths.push_back(health);

    sf::FloatRect new_Pos_;
    int rect_velocity_x = 150;
    int rect_velocity_y = 450;
    bool flag_y = false;
    bool flag_x = false;
    const float gridSize = 50.f;
    std::vector<sf::Sprite> walls;
    sf::Vector2f vel_;
    sf::Vector2i mos_Pos_;
    AnimatedSprite hero(10, "Character\\character.png");
    hero.setBounds(0, window.getSize().x, 0, window.getSize().y);
    hero.setSpeed(10,10,10);
    hero.setPosition(900,800);

    hero.add_animation_frame(sf::IntRect(150, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(200, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(250, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(300, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(350, 0, 50, 37)); // hero running frame 1
    hero.add_animation_frame(sf::IntRect(400, 0, 50, 37)); // hero running frame 1

    sf::Texture texture_background;
    if (!texture_background.loadFromFile("bckground.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    texture_background.setRepeated(true);
    sf::Sprite bckground;
    bckground.setTexture(texture_background);
    bckground.setTextureRect(sf::IntRect(0, 0, 1800, 900));

    //won
    sf::Texture texture_won;
    if (!texture_won.loadFromFile("won.png")) {
        return 1;
    }
    sf::Sprite won;
    won.setTexture(texture_won);
    won.scale(3.0, 2.3);

    //gameover
    sf::Texture texture_gameover;
    if (!texture_gameover.loadFromFile("gameover.png")) {
        return 1;
    }
    sf::Sprite gameover;
    gameover.setTexture(texture_gameover);
    gameover.scale(5.2, 3.0);

    //heart
    sf::Texture texture_heart;
    if(!texture_heart.loadFromFile("heart.png")) {
        return 1; }
    std::vector<sf::Sprite> hearts;
    sf::Sprite heart;
    heart.scale(0.06, 0.06);
    heart.setTexture(texture_heart);
    //heart_1
    heart.setPosition(sf::Vector2f(1680, 30));
    hearts.push_back(heart);
    //heart_2;
    heart.setPosition(sf::Vector2f(1710, 30));
    hearts.push_back(heart);
    //heart_3;
    heart.setPosition(sf::Vector2f(1740, 30));
    hearts.push_back(heart);
    //heart_4;
    heart.setPosition(sf::Vector2f(1770, 30));
    hearts.push_back(heart);

    sf::Texture texture_wall;
    if(!texture_wall.loadFromFile("wall.png")) {
        return 1; }
    sf::Sprite wall;
    wall.setTexture(texture_wall);
    wall.setTextureRect(sf::IntRect(0, 0, 35, 120));
    texture_wall.setRepeated(true);
    wall.setRotation(-90);

    //wall_1
    wall.setPosition(sf::Vector2f(50, 600));
    walls.push_back(wall);
    //wall_2;
    wall.setPosition(sf::Vector2f(300, 750));
    walls.push_back(wall);
    //wall_3;
    wall.setPosition(sf::Vector2f(550, 550));
    walls.push_back(wall);
    //wall_4;
    wall.setPosition(sf::Vector2f(800, 800));
    walls.push_back(wall);
    //wall_5;
    wall.setPosition(sf::Vector2f(1050, 650));
    walls.push_back(wall);
    //wall_6;
    wall.setPosition(sf::Vector2f(1300, 850));
    walls.push_back(wall);
    //wall_7;
    wall.setPosition(sf::Vector2f(1550, 700));
    walls.push_back(wall);

    // run the program as long as the window is open
    while (window.isOpen()) {
        hero.setTextureRect(sf::IntRect(50, 0, 50, 37));
        hero.setScale(1.5,1.5);

        sf::Time elapsed = clock.restart();
        mos_Pos_.x = sf::Mouse::getPosition(window).x / (int)gridSize;
        sf::Event event;

        for(auto &rec : rectangles) {
            rec.animate(elapsed);
            sf::FloatRect rectangle_bounds = rec.getGlobalBounds();
            if (rectangle_bounds.top >= window.getSize().y) {
                rec.setPosition(rectangle_bounds.left, 10);
            }

            if(rectangle_bounds.top<=0 || rectangle_bounds.top+rectangle_bounds.height>=window.getSize().y)
            {
                if(flag_y != true)  {
                    rect_velocity_y *= -1;
                }
                flag_y = true;
            }
            else
                flag_y = false;

            if(rectangle_bounds.left<=0 || rectangle_bounds.left+rectangle_bounds.width>=window.getSize().x)
            {
                if(flag_x!=true)
                {
                    rect_velocity_x *= -1;
                }
                flag_x = true;
            }
            else
                flag_x = false;
        }
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Keyboard::Key pressed;
        if(event.type == sf::Event::KeyPressed) {
            pressed = event.key.code;

            sf::FloatRect heroBounds = hero.getGlobalBounds();
            new_Pos_ = heroBounds;
            heroBounds.left += vel_.x;
            heroBounds.top += vel_.y;
            if (heroBounds.left >= window.getSize().x) {
                hero.setPosition(10, heroBounds.top);
            }
            if (heroBounds.left <= 0) {
                hero.setPosition(window.getSize().x, heroBounds.top);
            }

            if (hearts.size() == 0) {
                while (true) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                        window.close();
                    }
                    window.clear(sf::Color::White);
                    window.draw(gameover);
                    window.display();
                }
            }


            //            if(heroBounds.left > houseBounds.left
            //                    && heroBounds.left + heroBounds.width > houseBounds.left + recBounds.width){
            //                while (true) {
            //                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
            //                        window.close();
            //                    }
            //                    window.clear(sf::Color::White);
            //                    window.draw(won);

            //                    window.display();
            //                }
            //            }

            //        collision
            for(auto &rec : rectangles) {
                sf::FloatRect recBounds = rec.getGlobalBounds();

                for (size_t i = 0; i < hearts.size(); i--) {
                    if(recBounds.intersects(new_Pos_)){

                        //collision with enemy
                        //Top Collision
                        if(heroBounds.top < recBounds.top
                                && heroBounds.top  + heroBounds.height < recBounds.top  + recBounds.height
                                && heroBounds.left < recBounds.left + recBounds.width
                                && heroBounds.left + heroBounds.width > recBounds.left){
                            hearts.pop_back();
                            hero.setPosition(900,800);
                        }
                        //Bottom Collision
                        else if(heroBounds.top > recBounds.top
                                && heroBounds.top  + heroBounds.height > recBounds.top  + recBounds.height
                                && heroBounds.left < recBounds.left + recBounds.width
                                && heroBounds.left + heroBounds.width > recBounds.left){
                            hearts.pop_back();
                            hero.setPosition(900,800);
                        }
                        //Right Collision
                        if(heroBounds.left < recBounds.left
                                && heroBounds.left + heroBounds.width < recBounds.left + recBounds.width
                                && heroBounds.top < recBounds.top + recBounds.height
                                && heroBounds.top + recBounds.height > recBounds.top){
                            hearts.pop_back();
                            hero.setPosition(900,800);
                        }
                        //Left Collision
                        else if(heroBounds.left > recBounds.left
                                && heroBounds.left + heroBounds.width > recBounds.left + recBounds.width
                                && heroBounds.top < recBounds.top + recBounds.height
                                && heroBounds.top + heroBounds.height > recBounds.top){
                            hearts.pop_back();
                            hero.setPosition(900,800);
                        }
                    }
                    hero.moveInDirection(elapsed,pressed,hearts);
                }
            }
            //        collision with walls
            for(auto &wall : walls) {
                sf::FloatRect wallBounds = wall.getGlobalBounds();
                if(wallBounds.intersects(heroBounds)){
                    //top Collision
                    if(heroBounds.top < wallBounds.top
                            && heroBounds.top  + heroBounds.height < wallBounds.top  + wallBounds.height
                            && heroBounds.left < wallBounds.left + wallBounds.width
                            && heroBounds.left + heroBounds.width > wallBounds.left){
                        hero.setPosition(heroBounds.left, wallBounds.top - heroBounds.height);
                    }
                    //bottom Collision
                    else if(heroBounds.top > wallBounds.top
                            && heroBounds.top  + heroBounds.height > wallBounds.top  + wallBounds.height
                            && heroBounds.left < wallBounds.left + wallBounds.width
                            && heroBounds.left + heroBounds.width > wallBounds.left){
                        hero.setPosition(heroBounds.left, wallBounds.top + wallBounds.height);
                    }
                    //Right Collision
                    if(heroBounds.left < wallBounds.left
                            && heroBounds.left + heroBounds.width < wallBounds.left + wallBounds.width
                            && heroBounds.top < wallBounds.top + wallBounds.height
                            && heroBounds.top + heroBounds.height > wallBounds.top){
                        hero.setPosition(wallBounds.left - heroBounds.width, heroBounds.top);
                    }
                    //Left Collision
                    else if(heroBounds.left > wallBounds.left
                            && heroBounds.left + heroBounds.width > wallBounds.left + wallBounds.width
                            && heroBounds.top < wallBounds.top + wallBounds.height
                            && heroBounds.top + heroBounds.height > wallBounds.top){
                        hero.setPosition(wallBounds.left + wallBounds.width, heroBounds.top);
                    }
                }
            }
        }
        window.clear(sf::Color::Black);
        // draw everything here...
        window.draw(bckground);
        window.draw(hero);
        for(auto &heart_ : hearts) {
            window.draw(heart_);
        }
        //        for(auto &health : healths) {
        //            window.draw(health);
        //        }
        for(auto &rec : rectangles) {
            window.draw(rec);
        }
        for(auto &wall_ : walls) {
            window.draw(wall_);
        }
        window.display();
    }
    return 0;
}
