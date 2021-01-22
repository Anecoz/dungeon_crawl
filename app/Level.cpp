#include "Level.h"

#include <string>

#define SPRITE_SIZE 32
#define SPRITE_SCALE 4.0f
#define FLOOR1_X 736
#define FLOOR1_Y 272

Level::Level()
{
}

bool Level::init()
{
  bool res = _textureAtlas.loadFromFile(RESOURCE_PATH + std::string("/spritesheets/mainlevbuild.png"));
  if (!res) {
    return false;
  }

  // Try make a 5x5 grid
  for (unsigned x = 0; x < 10; ++x)
  for (unsigned y = 0; y < 10; ++y) {
    _testSprites.emplace_back();
    auto& sprite = _testSprites.back();

    sprite.setTexture(_textureAtlas);
    sprite.setTextureRect(sf::IntRect(FLOOR1_X, FLOOR1_Y, SPRITE_SIZE, SPRITE_SIZE));
    sprite.scale(SPRITE_SCALE, SPRITE_SCALE);
    sprite.setPosition(float(x * SPRITE_SIZE * SPRITE_SCALE), float(y * SPRITE_SIZE * SPRITE_SCALE));
  }

  return true;
}

void Level::render(sf::RenderWindow& window)
{
  for (auto& sprite: _testSprites) {
    window.draw(sprite);
  }  
}