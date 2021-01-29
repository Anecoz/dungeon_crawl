#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class DoorComponent : public ecs::Component
{
public:
  DoorComponent() {}
  ~DoorComponent() {}

  COMPONENT_CLONE_AND_ID(DoorComponent, DOOR_ID)
};