#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#define PI 3.14159265

float rand_float(float a, float b) {
  return ((b - a) * ((float)rand() / RAND_MAX)) + a;
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
    ((sf::Shape *)(this->shape))->setFillColor(color);
    ((sf::Shape *)(this->shape))->setPosition(position);
  };

  Object(sf::Drawable *shape) : shape(shape) {
    position = ((sf::Shape *)(this->shape))->getPosition();
    color = ((sf::Shape *)(this->shape))->getFillColor();
  };

  Object(float x, float y, sf::Color color = sf::Color::White,
         sf::Drawable *shape = new sf::RectangleShape(sf::Vector2f(10, 10))) {
    this->position = sf::Vector2f(x, y);
    this->color = color;
    this->shape = shape;
    ((sf::Shape *)(this->shape))->setFillColor(color);
    ((sf::Shape *)(this->shape))->setPosition(position);
  };

  Object(const Object &obj) {
    this->shape = obj.shape;
    this->color = obj.color;
    this->position = obj.position;
  }

  const sf::Drawable *get_shape() { return shape; };

  sf::Color get_color() { return ((sf::Shape *)shape)->getFillColor(); };

  sf::Vector2f get_position() { return ((sf::Shape *)shape)->getPosition(); };

  void set_position(sf::Vector2f position){ 
    this->position = position;
    ((sf::Shape *)shape)->setPosition(position);}
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
    ((sf::Text *)(this->shape))->setPosition(position);
    ((sf::Text *)(this->shape))->setCharacterSize(this->character_size);
    ((sf::Text *)(this->shape))->setFont(this->font);
    ((sf::Text *)(this->shape))->setFillColor(this->color);
    ((sf::Text *)(this->shape))->setString(this->value);
  };

  void set_value(std::string value) {
    this->value = value;
    ((sf::Text *)(this->shape))->setString(this->value);
  };

  void set_value(int value) {
    this->value = std::to_string(value);
    ((sf::Text *)(this->shape))->setString(this->value);
  };

  std::string get_value() { return this->value; };
};

class MovableObject : public Object {

protected:
  MovableObject(float x, float y, sf::Color color = sf::Color::White,
                sf::Drawable *shape = new sf::RectangleShape(sf::Vector2f(10,
                                                                          10)))
      : Object(x, y, color, shape) {
    start_position = position;
  };
  virtual void move() = 0;
  virtual void update_position() = 0;

  float speed;
  sf::Vector2f start_position;
  sf::Vector2f direction;
public:
  sf::Vector2f set_direction(float x, float y) {
    this->direction = sf::Vector2f(x, y);
  };

  sf::Vector2f get_direction() { return direction; };

  float get_speed() { return speed; };

  sf::Vector2f get_start_position() { return start_position; };

  void reset_position() { this->position = this->start_position; }
};

class newPlayer : public MovableObject {
private:
  int score;

  void move() { return; };

public:
  newPlayer(float x, float y, float speed, sf::Color color = sf::Color::White)
      : MovableObject(x, y, color, new RectangleShape(sf::Vector2f(20, 100))) {
    this->speed = speed;
    this->score = 0;
  };

  void move_down() { this->position.y += this->speed; };

  void move_up() { this->position.y -= this->speed; };

  void update_position() {
    if (this->position.y < 0)
      this->position.y = 0;

    if (this->position.y > 600)
      this->position.y = 600;
    ((sf::Shape *)(this->shape))->setPosition(this->position);
  };

  int get_score() { return this->score; };

  void plus_score() { this->score++; };
};

class newBall : public MovableObject {
private:
  void update_position(){};
public:
  newBall(float x, float y, float speed, sf::Color color = sf::Color::Red)
      : MovableObject(x, y, color, new CircleShape(15)){
        this->speed = speed;
        float angle = rand_float(0, 360 * PI / 180);
        direction = sf::Vector2f(sin(angle), cos(angle));
      };

    void move(){
      position = sf::Vector2f(position.x + direction.x * speed, position.y + direction.y * speed);
    };

    void update_position(newPlayer &player1, newPlayer &player2){
      if (position.y < 0) {
      direction.y *= -1;
    }
    if (position.y > 670) {
      direction.y *= -1;
    }
    if (((sf::CircleShape*)(this->get_shape()))->getGlobalBounds().intersects(
            ((sf::Shape*)(player1.get_shape()))->getGlobalBounds())) {
      float new_angle = rand_float(-45 * PI / 180, 45 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);
      position.x += 0.1;
    }
   if (((sf::CircleShape*)(this->get_shape()))->getGlobalBounds().intersects(
            ((sf::Shape*)(player2.get_shape()))->getGlobalBounds())) {
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

class Player {
public:
  Player(float x, float y) {
    position.x = x;
    position.y = y;
    player_bat.setFillColor(sf::Color::White);
    player_bat.setSize(sf::Vector2f(20, 100));
  }
  void update_position() {
    if (position.y < 0)
      position.y = 0;

    if (position.y > 600)
      position.y = 600;
    player_bat.setPosition(position);
  }
  void move_up() { position.y -= move_speed; }
  void move_down() { position.y += move_speed; }
  RectangleShape get_shape() { return player_bat; }
  void restart_position() {
    position.y = 250;
    player_bat.setPosition(position);
  }

  float move_speed = 5;
  sf::RectangleShape player_bat;
  sf::Vector2f position;
  int score = 0;
};

Player player1 = Player(50, 250);
Player player2 = Player(925, 250);

class Ball {
public:
  sf::CircleShape ball;
  sf::Vector2f position;
  sf::Vector2f direction;
  float move_speed = 7;
  Ball(float x, float y) {
    ball.setFillColor(sf::Color::Red);
    ball.setRadius(15);
    position.x = x;
    position.y = y;
    float angle = rand_float(0, 360 * PI / 180);
    direction.x = sin(angle);
    direction.y = cos(angle);
  }
  void move() {
    position.x += direction.x * move_speed;
    position.y += direction.y * move_speed;
  }
  void update_position() {
    if (position.y < 0) {
      direction.y *= -1;
    }
    if (position.y > 670) {
      direction.y *= -1;
    }
    if (ball.getGlobalBounds().intersects(
            player1.get_shape().getGlobalBounds())) {
      float new_angle = rand_float(-45 * PI / 180, 45 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);
      position.x += 0.1;
    }
    if (ball.getGlobalBounds().intersects(
            player2.get_shape().getGlobalBounds())) {
      float new_angle = rand_float(135 * PI / 180, 225 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);

      position.x -= 0.1;
    }
    if (position.x <= 25) {
      player1.restart_position();
      player2.restart_position();
      float new_angle = rand_float(-45 * PI / 180, 45 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);
      position.x = 500;
      position.y = 350;
      player2.score++;
    }
    if (position.x >= 950) {
      player1.restart_position();
      player2.restart_position();
      float new_angle = rand_float(135 * PI / 180, 225 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);
      position.x = 500;
      position.y = 350;
      player1.score++;
    }
    ball.setPosition(position);
  }
  sf::CircleShape get_shape() { return ball; }
};

int main() {
  newPlayer plr(50, 100, 5);

  sf::Music music;
  if (!music.openFromFile("music.ogg"))
    return -1; // error
  srand(time(NULL));
  music.play();
  music.setLoop(true);
  sf::Font font;
  ScoreText scores[] = {ScoreText(player1.score, 250, 20),
                        ScoreText(player2.score, 750, 20)};

  sf::RenderWindow window(sf::VideoMode(1000, 700), "Pong");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  Object game_field_elements[] = {
      Object(0, 0, sf::Color::White,
             new sf::RectangleShape(sf::Vector2f(10, 700))),
      Object(500, 0, sf::Color::White,
             new sf::RectangleShape(sf::Vector2f(10, 700))),
      Object(990, 0, sf::Color::White,
             new sf::RectangleShape(sf::Vector2f(10, 700)))};

  player1.update_position();
  player2.update_position();

  Ball ball = Ball(500, 350);

  while (window.isOpen()) {
    scores[0].set_value(player1.score);
    scores[1].set_value(player2.score);
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
      player2.move_up();
      player2.update_position();
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      player2.move_down();
      plr.move_up();
      plr.update_position();
      player2.update_position();
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::W)) {
      player1.move_up();
      player1.update_position();
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::S)) {
      player1.move_down();
      player1.update_position();
    }
    if (Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }
    ball.move();
    ball.update_position();

    window.clear();

    window.draw(player1.get_shape());
    window.draw(player2.get_shape());
    for (int i = 0; i < 3; i++) {
      window.draw(*game_field_elements[i].get_shape());
    };
    for (int i = 0; i < 2; i++) {
      window.draw(*scores[i].get_shape());
    }
    window.draw(ball.get_shape());

    window.draw(*plr.get_shape());

    window.display();
  }

  return 0;
}