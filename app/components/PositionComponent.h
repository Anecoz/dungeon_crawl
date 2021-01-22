#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"

class PositionComponent : public ecs::Component
{
public:
  PositionComponent() {}
  PositionComponent(double x, double y) : _x(x), _y(y) {}
  ~PositionComponent() {}

  ecs::Component* clone() override {
    auto comp = new PositionComponent();
    comp->_x = _x;
    comp->_y = _y;
    return comp;
  }

  ecs::ComponentID id() override {
    return POS_ID;
  }

  double _x = 0.0;
  double _y = 0.0;
};