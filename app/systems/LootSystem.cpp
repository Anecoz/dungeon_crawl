#include "LootSystem.h"

#include "../ecs/Engine.h"
#include "../components/ComponentIds.h"
#include "../components/EventComponent.h"
#include "../components/LootComponent.h"

#include <iostream>

void LootSystem::run(ecs::Engine& engine)
{
  auto events = engine.getEntitiesWithComp(EVENT_ID);
  for (auto it = events.begin(); it != events.end();) {
    auto eventComp = static_cast<EventComponent*>((*it)->getComp(EVENT_ID));
    if (eventComp->_type != EventComponent::Type::Death) {
      ++it;
      continue;
    }
    
    std::cout << "Received loot event, loot is: " << eventComp->_lootComp->_coolLoot << std::endl;

    // Remove the event from the engine once we've "read it"
    engine.removeEntity((*it)->id());
    it = events.erase(it);
  }
}