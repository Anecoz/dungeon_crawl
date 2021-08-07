#include "CollisionSystem.h"
#include "../components/PlayerInputComponent.h"
#include "../components/PositionComponent.h"
#include "../components/LootComponent.h"
#include "../components/StatComponent.h"
#include "../components/EventComponent.h"

#include "../Helpers.h"

#include "../ecs/Engine.h"

#include <iostream>

static void applyPickup(ecs::Entity* player, ecs::Entity* pickup)
{
  auto lootComp = static_cast<LootComponent*>(pickup->getComp(LOOT_ID));
  if (lootComp->_type == LootComponent::Type::Health) {
    auto statComp = static_cast<StatComponent*>(player->getComp(STAT_ID));
    statComp->_health += lootComp->_hp;
    // Clamp the hp
    if (statComp->_health > (int)statComp->_maxHealth) statComp->_health = statComp->_maxHealth;
  }
}

void CollisionSystem::run(ecs::Engine& engine)
{
  // Basically check if a player has run into a pickup item (for now atleast)
  auto playerEntities = engine.getEntitiesWithComp(PLAYER_INPUT_ID);
  if (playerEntities.empty()) return;

  auto player = playerEntities[0];
  auto playerPosComp = static_cast<PositionComponent*>(player->getComp(POS_ID));

  // Stuff that we're interested in colliding with
  auto pickups = engine.getEntitiesWithComp(PICKUP_ID);
  auto doors = engine.getEntitiesWithComp(DOOR_ID);

  for (auto pickup: pickups) {
    auto pickupPosComp = static_cast<PositionComponent*>(pickup->getComp(POS_ID));
    if (helpers::distance(playerPosComp, pickupPosComp) <= 0.9) {
      applyPickup(player, pickup);
      engine.removeEntity(pickup->id());
    }
  }

  for (auto door: doors) {
    auto doorPosComp = static_cast<PositionComponent*>(door->getComp(POS_ID));
    if (helpers::distance(playerPosComp, doorPosComp) <= 0.9) {
      // Create an event for the level system to react to
      auto eventEnt = std::make_unique<ecs::Entity>();
      auto eventComp = std::make_unique<EventComponent>();
      eventComp->_type = EventComponent::Type::DoorReached;
      eventEnt->addComp(std::move(eventComp));
      engine.addEntity(std::move(eventEnt));
    }
  }
}