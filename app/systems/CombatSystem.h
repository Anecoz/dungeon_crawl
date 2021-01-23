#pragma once

#include "../ecs/System.h"
#include "../ecs/Entity.h"

#include <vector>

class CombatSystem : public ecs::System
{
public:
  CombatSystem() = default;
  ~CombatSystem() = default;

  void run(ecs::Engine& engine) override;

private:
  bool _ongoingFight = false;
  std::vector<ecs::EntityID> _fighters;
};