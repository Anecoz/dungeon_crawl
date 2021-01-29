#pragma once

#include "../ecs/Component.h"
#include "../Orientation.h"
#include "ComponentIds.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

class SpriteComponent : public ecs::Component
{
public:
  SpriteComponent(
    const std::string& texPath,
    double scaleX,
    double scaleY,
    unsigned left,
    unsigned top,
    unsigned width,
    unsigned height) 
    : _orientation(Orientation::North)
    , _texPath(texPath)
    , _scaleX(scaleX)
    , _scaleY(scaleY)
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
    _sprite.scale(static_cast<float>(_scaleX), static_cast<float>(_scaleY));
    _sprite.setOrigin(static_cast<float>(_width)/2.0f, static_cast<float>(_height)/2.0f);
  }

  ~SpriteComponent() {}

  ecs::Component* clone() override {
    auto comp = new SpriteComponent(_texPath, _scaleX, _scaleY, _left, _top, _width, _height);
    comp->_orientation = _orientation;
    return comp;
  }

  ecs::ComponentID id() override {
    return SPRITE_ID;
  }

  double rotation() {
    switch (_orientation) {
      case Orientation::North:
        return 0.0;
      case Orientation::NorthEast:
        return 45.0;
      case Orientation::East:
        return 90.0;
      case Orientation::SouthEast:
        return 135.0;
      case Orientation::South:
        return 180.0;
      case Orientation::SouthWest:
        return 225.0;
      case Orientation::West:
        return 270.0;
      case Orientation::NorthWest:
        return 315.0;
    }
    return 0.0;
  }

  sf::Texture _texture;
  sf::Sprite _sprite;

  Orientation _orientation;

private:
  std::string _texPath;
  double _scaleX, _scaleY;
  unsigned _left, _top, _width, _height;
};