#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#define PI 3.14159265

float rand_float(float a, float b) {
    return ((b - a) * ((float) rand() / RAND_MAX)) + a;
}

using namespace sf;

class Object {

protected:
    sf::Color color;
    sf::Vector2f position;
    sf::Drawable *shape;

public:
    Object() : position(sf::Vector2f(0, 0)), color(sf::Color::White) {
        this->shape = new sf::RectangleShape(sf::Vector2f(10, 10));
        ((sf::Shape *) (this->shape))->setFillColor(color);
        ((sf::Shape *) (this->shape))->setPosition(position);
    };

    Object(sf::Drawable *shape) : shape(shape) {
        position = ((sf::Shape *) (this->shape))->getPosition();
        color = ((sf::Shape *) (this->shape))->getFillColor();
    };

    Object(float x, float y, sf::Color color = sf::Color::White,
           sf::Drawable *shape = new sf::RectangleShape(sf::Vector2f(10, 10))) {
        this->position = sf::Vector2f(x, y);
        this->color = color;
        this->shape = shape;
        ((sf::Shape *) (this->shape))->setFillColor(color);
        ((sf::Shape *) (this->shape))->setPosition(position);
    };

    const sf::Drawable *get_shape() { return this->shape; };

    void set_shape(sf::Drawable *shape) {
        this->shape = shape;
        this->color = ((sf::Shape *) (this->shape))->getFillColor();
        this->position = ((sf::Shape *) (this->shape))->getPosition();
    }

    sf::Color get_color() { return this->color; };

    void set_color(sf::Color color) {
        this->color = color;
        ((sf::Shape *) (shape))->setFillColor(color);
    }

    sf::Vector2f get_position() { return ((sf::Shape *) shape)->getPosition(); };

    void set_position(sf::Vector2f position) {
        this->position = position;
        ((sf::Shape *) shape)->setPosition(position);
    }
};

class ScoreText : public Object {

private:
    std::string value;
    sf::Font font;
    int character_size;

public:
    ScoreText(int value, float x, float y, std::string font_path = "font.ttf",
              int character_size = 30)
            : value(std::to_string(value)), character_size(character_size) {
        this->shape = new sf::Text;
        this->font.loadFromFile(font_path);
        this->color = color;
        this->position = sf::Vector2f(x, y);
        ((sf::Text *) (this->shape))->setPosition(position);
        ((sf::Text *) (this->shape))->setCharacterSize(this->character_size);
        ((sf::Text *) (this->shape))->setFont(this->font);
        ((sf::Text *) (this->shape))->setFillColor(this->color);
        ((sf::Text *) (this->shape))->setString(this->value);
    };

    void set_value(std::string value) {
        this->value = value;
        ((sf::Text *) (this->shape))->setString(this->value);
    };

    void set_value(int value) {
        this->value = std::to_string(value);
        ((sf::Text *) (this->shape))->setString(this->value);
    };

    sf::Font get_font() { return this->font; };

    void set_font(sf::Font font) {
        this->font = font;
        ((sf::Text *) (this->shape))->setFont(this->font);
    };

    std::string get_value() { return this->value; };
};

class MovableObject : public Object {

protected:
    MovableObject(float x, float y, float speed = 0,
                  sf::Color color = sf::Color::White,
                  sf::Drawable *shape = new sf::RectangleShape(sf::Vector2f(10,
                                                                            10)))
            : Object(x, y, color, shape), speed(speed), start_position(position) {};

    virtual void move() = 0;

    virtual void update_position() = 0;

    float speed;
    sf::Vector2f start_position;
    sf::Vector2f direction;

public:
    void set_direction(sf::Vector2f direction) { this->direction = direction; };

    void set_direction(float x, float y) {
        this->direction = sf::Vector2f(x, y);
    };

    sf::Vector2f get_direction() { return direction; };

    float get_speed() const { return this->speed; };

    void set_speed(float speed) { this->speed = speed; };

    sf::Vector2f get_start_position() { return start_position; };

    void reset_position() {
        this->position = this->start_position;
        ((sf::Shape *) (shape))->setPosition(position);
    };
};

class Player : public MovableObject {
private:
    int score;

    void move() override {};

public:
    Player(float x, float y, float speed, sf::Color color = sf::Color::White)
            : MovableObject(x, y, speed, color,
                            new RectangleShape(sf::Vector2f(20, 100))),
              score(0) {};

    void move_down() { this->position.y += this->speed; };

    void move_up() { this->position.y -= this->speed; };

    void update_position() override {
        if (this->position.y < 0)
            this->position.y = 0;

        if (this->position.y > 600)
            this->position.y = 600;
        ((sf::Shape *) (this->shape))->setPosition(this->position);
    };

    int get_score() const { return this->score; };

    void plus_score() { this->score++; };
};

class Ball : public MovableObject {
private:
    void update_position() override {};

public:
    Ball(float x, float y, float speed, sf::Color color = sf::Color::Red)
            : MovableObject(x, y, speed, color, new CircleShape(15)) {
        float angle = rand_float(0, 360 * PI / 180);
        direction = sf::Vector2f(sin(angle), cos(angle));
    };

    void move() override {
        position = sf::Vector2f(position.x + direction.x * speed,
                                position.y + direction.y * speed);
    };

    void update_position(Player &player1, Player &player2) {
        if (position.y < 0) {
            direction.y *= -1;
        }
        if (position.y > 670) {
            direction.y *= -1;
        }
        if (((sf::CircleShape *) (this->get_shape()))
                ->getGlobalBounds()
                .intersects(
                        ((sf::Shape *) (player1.get_shape()))->getGlobalBounds())) {
            float new_angle = rand_float(-45 * PI / 180, 45 * PI / 180);
            direction.x = cos(new_angle);
            direction.y = sin(new_angle);
            position.x += 0.1;
        }
        if (((sf::CircleShape *) (this->get_shape()))
                ->getGlobalBounds()
                .intersects(
                        ((sf::Shape *) (player2.get_shape()))->getGlobalBounds())) {
            float new_angle = rand_float(135 * PI / 180, 225 * PI / 180);
            direction.x = cos(new_angle);
            direction.y = sin(new_angle);

            position.x -= 0.1;
        }
        if (position.x <= 25) {
            player1.reset_position();
            player2.reset_position();
            float new_angle = rand_float(-45 * PI / 180, 45 * PI / 180);
            direction.x = cos(new_angle);
            direction.y = sin(new_angle);
            position.x = 500;
            position.y = 350;
            player2.plus_score();
        }
        if (position.x >= 950) {
            player1.reset_position();
            player2.reset_position();
            float new_angle = rand_float(135 * PI / 180, 225 * PI / 180);
            direction.x = cos(new_angle);
            direction.y = sin(new_angle);
            position.x = 500;
            position.y = 350;
            player1.plus_score();
        }
        this->set_position(position);
    }
};

int main() {
    srand(time(NULL));
    Object **game_field_objects = new Object *[8];
    game_field_objects[0] = new Object(0, 0, sf::Color::White, new sf::RectangleShape(sf::Vector2f(10, 700)));
    game_field_objects[1] = new Object(500, 0, sf::Color::White, new sf::RectangleShape(sf::Vector2f(10, 700)));
    game_field_objects[2] = new Object(990, 0, sf::Color::White, new sf::RectangleShape(sf::Vector2f(10, 700)));
    game_field_objects[3] = new Player(50, 250, 5);
    game_field_objects[4] = new Player(925, 250, 5);
    game_field_objects[5] = new ScoreText(((Player *) (game_field_objects[3]))->get_score(), 250, 20);
    game_field_objects[6] = new ScoreText(((Player *) (game_field_objects[4]))->get_score(), 750, 20);
    game_field_objects[7] = new Ball(500, 350, 8);

    sf::Music music;
    if (!music.openFromFile("music.ogg"))
        return -1; // error

    music.play();
    music.setLoop(true);
    sf::Font font;

    sf::RenderWindow window(sf::VideoMode(1000, 700), "Pong");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(120);
    while (window.isOpen()) {
        ((ScoreText *) (game_field_objects[5]))
                ->set_value(((Player *) (game_field_objects[3]))->get_score());
        ((ScoreText *) (game_field_objects[6]))
                ->set_value(((Player *) (game_field_objects[4]))->get_score());
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::W)) {
            ((Player *) (game_field_objects[3]))->move_up();
            ((Player *) (game_field_objects[3]))->update_position();
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::S)) {
            ((Player *) (game_field_objects[3]))->move_down();
            ((Player *) (game_field_objects[3]))->update_position();
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            ((Player *) (game_field_objects[4]))->move_up();
            ((Player *) (game_field_objects[4]))->update_position();
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            ((Player *) (game_field_objects[4]))->move_down();
            ((Player *) (game_field_objects[4]))->update_position();
        }
        if (Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        ((Ball *) (game_field_objects[7]))->move();
        ((Ball *) (game_field_objects[7]))->update_position(*((Player *) game_field_objects[3]),*((Player *) game_field_objects[4]));
        window.clear();

        for (int i = 0; i < 8; i++) {
            window.draw(*game_field_objects[i]->get_shape());
        };

        window.display();
    }
    return 0;
}