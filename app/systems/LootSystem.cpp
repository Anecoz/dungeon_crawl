#include "LootSystem.h"

#include "../ecs/Engine.h"
#include "../ecs/Entity.h"
#include "../components/ComponentIds.h"
#include "../components/EventComponent.h"
#include "../components/LootComponent.h"
#include "../components/PickupComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/PositionComponent.h"

#include "../Constants.h"

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
    std::cout << "Received loot event" << std::endl;

    // Spawn a pickup entity with the loot component
    auto pickup = std::make_unique<ecs::Entity>();
    
    auto spriteComp = std::make_unique<SpriteComponent>(
      RESOURCE_PATH + std::string("pickups/hp_pickup.png"),
      1.0,
      1.0,
      0, 0,
      32, 32);

    auto pickupComp = std::make_unique<PickupComponent>();
    
    pickup->addComp(std::move(spriteComp));
    pickup->addComp(std::move(pickupComp));
    pickup->addComp(std::move(eventComp->_posComp));
    pickup->addComp(std::move(eventComp->_lootComp));

    engine.addEntity(std::move(pickup));

    // Remove the event from the engine once we've "read it"
    engine.removeEntity((*it)->id());
    it = events.erase(it);
  }
}