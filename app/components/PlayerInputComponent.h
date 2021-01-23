#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class PlayerInputComponent : public ecs::Component
{
public:
  PlayerInputComponent() {}
  ~PlayerInputComponent() {}

  COMPONENT_CLONE_AND_ID(PlayerInputComponent, PLAYER_INPUT_ID)
};