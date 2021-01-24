#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"
#include "../Orientation.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class AbilityComponent : public ecs::Component
{
public:
  struct Ability
  {
    Ability(const std::string& texPath, int damage, unsigned apCost)
      : _type(Type::Damage)
      , _damage(damage)
      , _movedOrientation(Orientation::North)
      , _apCost(apCost)
      , _texPath(texPath)
    {
      _texture.loadFromFile(texPath);
      _sprite.setTexture(_texture);
    }

    Ability(Ability&& other)
      : _type(other._type)
      , _damage(std::move(other._damage))
      , _movedOrientation(other._movedOrientation)
      , _apCost(std::move(other._apCost))
      , _texPath(std::move(other._texPath))
      , _texture(std::move(other._texture))
    {
      _sprite.setTexture(_texture);
    }

    enum class Type
    {
      Damage,
      Move
    } _type;

    // Damage type stuff
    int _damage;

    // Move type stuff
    Orientation _movedOrientation;

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
      clonedAb._type = ability._type;
      clonedAb._movedOrientation = ability._movedOrientation;
      comp->_abilities.emplace_back(std::move(clonedAb));
    }
    return comp;
  }

  ecs::ComponentID id() override {
    return ABILITY_ID;
  }

  std::vector<Ability> _abilities;
};