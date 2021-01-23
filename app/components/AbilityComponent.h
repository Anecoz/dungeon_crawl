#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

#include <vector>

class AbilityComponent : public ecs::Component
{
public:
  struct Ability
  {
    unsigned _damage;
  };

  AbilityComponent() = default;
  AbilityComponent(std::vector<Ability>&& abilities) : _abilities(std::move(abilities)) {}
  ~AbilityComponent() {}

  COMPONENT_CLONE_AND_ID(AbilityComponent, ABILITY_ID)

  std::vector<Ability> _abilities;
};