#pragma once

#include "../ecs/System.h"

class LevelSystem : public ecs::System
{
public:
  LevelSystem() = default;
  ~LevelSystem() = default;

  void run(ecs::Engine& engine) override;
};