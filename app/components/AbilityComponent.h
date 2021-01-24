#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class AbilityComponent : public ecs::Component
{
public:
  struct Ability
  {
    Ability(const std::string& texPath, int damage, unsigned apCost)
      : _damage(damage)
      , _apCost(apCost)
      , _texPath(texPath)
    {
      _texture.loadFromFile(texPath);
      _sprite.setTexture(_texture);
    }

    Ability(Ability&& other)
      : _damage(std::move(other._damage))
      , _apCost(std::move(other._apCost))
      , _texPath(std::move(other._texPath))
    {
      _texture.loadFromFile(_texPath);
      _sprite.setTexture(_texture);
    }

    Ability& operator=(Ability&& other)
    {
      if (this != &other) {
        _damage = std::move(other._damage);
        _apCost = std::move(other._apCost);
        _texPath = std::move(other._texPath);
        _texture.loadFromFile(_texPath);
        _sprite.setTexture(_texture);
      }
      return *this;
    }

    Ability(const Ability& other)
      : _damage(other._damage)
      , _apCost(other._apCost)
      , _texPath(other._texPath)
    {
      _texture.loadFromFile(_texPath);
      _sprite.setTexture(_texture);
    }

    Ability& operator=(const Ability& other) 
    {
      if (this != &other) {
        _damage = other._damage;
        _apCost = other._apCost;
        _texPath = other._texPath;
        _texture.loadFromFile(_texPath);
        _sprite.setTexture(_texture);
      }
      return *this;
    }

    int _damage;
    unsigned _apCost;

    std::string _texPath;
    sf::Texture _texture;
    sf::Sprite _sprite;
  };

  AbilityComponent() = default;
  ~AbilityComponent() {}

  ecs::Component* clone() override {
    auto comp = new AbilityComponent();
    for (auto& ability: _abilities) {
      Ability clonedAb(ability._texPath, ability._damage, ability._apCost);
      comp->_abilities.emplace_back(std::move(clonedAb));
    }
    return comp;
  }

  ecs::ComponentID id() override {
    return ABILITY_ID;
  }

  std::vector<Ability> _abilities;
};