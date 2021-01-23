#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class AIComponent : public ecs::Component
{
public:
  AIComponent() {}
  ~AIComponent() {}

  COMPONENT_CLONE_AND_ID(AIComponent, AI_ID)
};