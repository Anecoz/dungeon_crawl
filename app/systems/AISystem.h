#pragma once

#include "../ecs/System.h"

class AISystem : public ecs::System
{
public:
  AISystem() = default;
  ~AISystem() = default;

  void run(ecs::Engine& engine) override;
};