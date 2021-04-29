#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#define PI 3.14159265

float rand_float(float a, float b) {
  return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

using namespace sf;

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
    float angle = rand_float(0, 360 * PI/180);
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
      position.x += 10.1;
    }
    if (ball.getGlobalBounds().intersects(
            player2.get_shape().getGlobalBounds())) {
      float new_angle = rand_float(135 * PI / 180, 225 * PI / 180);
      direction.x = cos(new_angle);
      direction.y = sin(new_angle);

      position.x -= 10.1;
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
  sf::Music music;
  if (!music.openFromFile("music.ogg"))
    return -1; // error
  srand(time(NULL));
  music.play();
  music.setLoop(true);
  sf::Font font;
  font.loadFromFile("font.ttf");
  sf::Text score1(std::to_string(player1.score), font);
  sf::Text score2(std::to_string(player2.score), font);
  score1.setCharacterSize(30);
  score2.setCharacterSize(30);
  score1.setColor(sf::Color::White);
  score2.setColor(sf::Color::White);
  score1.setPosition(250, 20);
  score2.setPosition(750, 20);
  sf::RenderWindow window(sf::VideoMode(1000, 700), "Pong");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  RectangleShape line;
  line.setSize(sf::Vector2f(10, 700));
  line.setPosition(500, 0);

  sf::RectangleShape left_border_line;
  left_border_line.setSize(sf::Vector2f(10, 700));
  left_border_line.setPosition(0, 0);

  sf::RectangleShape right_border_line;
  right_border_line.setSize(sf::Vector2f(10, 700));
  right_border_line.setPosition(985, 0);

  player1.update_position();
  player2.update_position();

  Ball ball = Ball(500, 350);

  while (window.isOpen()) {
    score1.setString(std::to_string(player1.score));
    score2.setString(std::to_string(player2.score));
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
    window.draw(line);
    window.draw(left_border_line);
    window.draw(right_border_line);
    window.draw(ball.get_shape());
    window.draw(score1);
    window.draw(score2);

    window.display();
  }

  return 0;
}