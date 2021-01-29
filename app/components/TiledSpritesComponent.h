#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>

class TiledSpritesComponent : public ecs::Component
{
public:
  struct SpriteTile
  {
    sf::Sprite _sprite;
    double _x;
    double _y;
  };

  TiledSpritesComponent(
    const std::string& texPath,
    unsigned numTiles,
    double scaleX,
    double scaleY,
    unsigned left,
    unsigned top,
    unsigned width,
    unsigned height) 
    : _texPath(texPath)
    , _numTiles(numTiles)
    , _scaleX(scaleX)
    , _scaleY(scaleY)
    , _left(left)
    , _top(top)
    , _width(width)
    , _height(height)
  {
    if (!_textureAtlas.loadFromFile(_texPath)) {
      std::cerr << "Could not load texture from " << _texPath << "!" << std::endl;
    }

    for (unsigned x = 0; x < _numTiles; ++x)
    for (unsigned y = 0; y < _numTiles; ++y) {
      _sprites.emplace_back();
      auto& spriteTile = _sprites.back();

      spriteTile._sprite.setTexture(_textureAtlas);
      spriteTile._sprite.setTextureRect(sf::IntRect(_left, _top, _width, _height));
      spriteTile._sprite.scale(static_cast<float>(_scaleX), static_cast<float>(_scaleY));
      spriteTile._x = x * _width * _scaleX;
      spriteTile._y = y * _height * _scaleY;
    }
  }

  ~TiledSpritesComponent() {}

  ecs::Component* clone() override {
    auto comp = new TiledSpritesComponent(_texPath, _numTiles, _scaleX, _scaleY, _left, _top, _width, _height);
    return comp;
  }

  ecs::ComponentID id() override {
    return TILED_SPRITE_ID;
  }

  sf::Texture _textureAtlas;
  std::vector<SpriteTile> _sprites;
private:
  std::string _texPath;
  unsigned _numTiles;
  double _scaleX, _scaleY;
  unsigned _left, _top, _width, _height;
};