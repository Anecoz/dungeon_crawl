#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class PositionComponent : public ecs::Component
{
public:
  PositionComponent() {}
  PositionComponent(double x, double y) : _x(x), _y(y) {}
  ~PositionComponent() {}

  COMPONENT_CLONE_AND_ID(PositionComponent, POS_ID)

  double _x = 0.0;
  double _y = 0.0;
};