#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class StatComponent : public ecs::Component
{
public:
  StatComponent() {}
  StatComponent(int health) 
    : _health(health)
    , _maxHealth(static_cast<unsigned>(health)) 
  {}
  ~StatComponent() {}

  COMPONENT_CLONE_AND_ID(StatComponent, STAT_ID)

  int _health;
  unsigned _maxHealth;
  unsigned _ap = 0;
};