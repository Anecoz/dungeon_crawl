#include "LevelSystem.h"

#include "../components/ComponentIds.h"
#include "../components/EventComponent.h"
#include "../components/CombatComponent.h"
#include "../components/TiledSpritesComponent.h"
#include "../components/PickupComponent.h"
#include "../components/DoorComponent.h"
#include "../ecs/Engine.h"
#include "../ecs/Entity.h"

#include <iostream>

static void clearLevel(ecs::Engine& engine)
{
  std::vector<ecs::EntityID> toRemove;

  auto combatEnts = engine.getEntitiesWithComp(COMBAT_ID);
  for (auto ent: combatEnts) {
    auto combatComp = static_cast<CombatComponent*>(ent->getComp(COMBAT_ID));
    if (combatComp->_faction == CombatComponent::Faction::Enemy) {
      toRemove.push_back(ent->id());
    }
  }

  auto levelEnts = engine.getEntitiesWithComp(TILED_SPRITE_ID);
  for (auto ent: levelEnts) {
    toRemove.push_back(ent->id());
  }

  auto pickupEnts = engine.getEntitiesWithComp(PICKUP_ID);
  for (auto ent: pickupEnts) {
    toRemove.push_back(ent->id());
  }

  auto doorEnts = engine.getEntitiesWithComp(DOOR_ID);
  for (auto ent: doorEnts) {
    toRemove.push_back(ent->id());
  }

  // Finally remove
  for (auto id: toRemove) {
    engine.removeEntity(id);
  }
}

void LevelSystem::run(ecs::Engine& engine)
{
  // Check if there is a door reached event to react to
  std::vector<ecs::EntityID> pendingDelete;
  auto eventEnts = engine.getEntitiesWithComp(EVENT_ID);
  for (auto event: eventEnts) {
    auto eventComp = static_cast<EventComponent*>(event->getComp(EVENT_ID));
    if (eventComp->_type == EventComponent::Type::DoorReached) {
      std::cout << "Door event received" << std::endl;
      pendingDelete.push_back(event->id());

      // Remove all mobs and the current level entity
      // Generate a new level and place player at start
      // Spawn some random mobs
      clearLevel(engine);
      break;
    }
  }

  for (auto id: pendingDelete) {
    engine.removeEntity(id);
  }
}