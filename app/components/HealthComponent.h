#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class HealthComponent : public ecs::Component
{
public:
  HealthComponent() {}
  HealthComponent(unsigned health) : _health(health), _maxHealth(health) {}
  ~HealthComponent() {}

  ecs::Component* clone() override {
    auto comp = new HealthComponent();
    comp->_health = _health;
    comp->_maxHealth = _maxHealth;
    return comp;
  }

  ecs::ComponentID id() override {
    return HEALTH_ID;
  }

  unsigned _health;
  unsigned _maxHealth;
};