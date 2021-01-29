#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class PickupComponent : public ecs::Component
{
public:
  PickupComponent() {}
  ~PickupComponent() {}

  COMPONENT_CLONE_AND_ID(PickupComponent, PICKUP_ID)
};