#pragma once

#include "../ecs/System.h"

class CollisionSystem : public ecs::System
{
public:
  CollisionSystem() = default;
  ~CollisionSystem() = default;

  void run(ecs::Engine& engine) override;
};