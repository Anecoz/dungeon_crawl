#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "ComponentIds.h"

class CombatComponent : public ecs::Component
{
public:
  enum class Faction {
    Player,
    Enemy
  } _faction;

  CombatComponent() {}
  CombatComponent(Faction faction) : _faction(faction) {}
  ~CombatComponent() {}

  COMPONENT_CLONE_AND_ID(CombatComponent, COMBAT_ID)

  bool _inCombat = false;
  bool _awaitingInput = false;
  int _chosenAbility = -1; // -1 means.. nothing chosen 
  ecs::EntityID _target;  
};