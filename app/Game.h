#pragma once

#include <SFML/Graphics.hpp>

class Game
{
public:
  Game();
  ~Game() = default;

  bool init();
  void run();

private:
  sf::RenderWindow _window;
};