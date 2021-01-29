#pragma once

#include "../ecs/System.h"

class LootSystem : public ecs::System
{
public:
  LootSystem() = default;
  ~LootSystem() = default;

  void run(ecs::Engine& engine) override;
};