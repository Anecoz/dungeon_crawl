#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class LootComponent : public ecs::Component
{
public:
  LootComponent() {}
  ~LootComponent() {}

  COMPONENT_CLONE_AND_ID(LootComponent, LOOT_ID)

  int _coolLoot = 420;
};