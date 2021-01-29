#pragma once

#include "../ecs/Component.h"
#include "ComponentIds.h"
#include "LootComponent.h"
#include "PositionComponent.h"

#include <memory>

class EventComponent : public ecs::Component
{
public:
  EventComponent() {}
  ~EventComponent() {}

  ecs::Component* clone() override {
    auto comp = new EventComponent();
    comp->_lootComp = std::unique_ptr<LootComponent>(static_cast<LootComponent*>(_lootComp->clone()));
    comp->_posComp = std::unique_ptr<PositionComponent>(static_cast<PositionComponent*>(_posComp->clone()));
    return comp;
  }

  ecs::ComponentID id() override {
    return EVENT_ID;
  }

  enum class Type
  {
    Death
  } _type;

  std::unique_ptr<LootComponent> _lootComp;
  std::unique_ptr<PositionComponent> _posComp;

};