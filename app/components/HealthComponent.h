#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class HealthComponent : public ecs::Component
{
public:
  HealthComponent() {}
  HealthComponent(unsigned health) : _health(health), _maxHealth(health) {}
  ~HealthComponent() {}

  COMPONENT_CLONE_AND_ID(HealthComponent, HEALTH_ID)

  unsigned _health;
  unsigned _maxHealth;
};