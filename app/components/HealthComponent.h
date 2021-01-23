#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class HealthComponent : public ecs::Component
{
public:
  HealthComponent() {}
  HealthComponent(int health) : _health(health), _maxHealth(static_cast<unsigned>(health)) {}
  ~HealthComponent() {}

  COMPONENT_CLONE_AND_ID(HealthComponent, HEALTH_ID)

  int _health;
  unsigned _maxHealth;
};