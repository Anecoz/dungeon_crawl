#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Level
{
public:
  Level();

  bool init();
  void render(sf::RenderWindow& window);

private:
  sf::Texture _textureAtlas;
  std::vector<sf::Sprite> _testSprites;
};