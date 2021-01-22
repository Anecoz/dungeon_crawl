#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class SpriteComponent : public ecs::Component
{
public:
  SpriteComponent(
    const std::string& texPath,
    double scale,
    unsigned left,
    unsigned top,
    unsigned width,
    unsigned height) 
    : _texPath(texPath)
    , _scale(scale)
    , _left(left)
    , _top(top)
    , _width(width)
    , _height(height)
  {
    if (!_texture.loadFromFile(_texPath)) {
      std::cerr << "Could not load texture from " << _texPath << "!" << std::endl;
    }
    _sprite.setTexture(_texture);
    _sprite.setTextureRect(sf::IntRect(_left, _top, _width, _height));
    _sprite.scale(static_cast<float>(_scale), static_cast<float>(_scale));
  }

  ~SpriteComponent() {}

  ecs::Component* clone() override {
    auto comp = new SpriteComponent(_texPath, _scale, _left, _top, _width, _height);
    return comp;
  }

  ecs::ComponentID id() override {
    return SPRITE_ID;
  }

  sf::Texture _texture;
  sf::Sprite _sprite;
private:
  std::string _texPath;
  double _scale;
  unsigned _left, _top, _width, _height;
};