#include "CollisionSystem.h"
#include "../components/PlayerInputComponent.h"
#include "../components/PositionComponent.h"
#include "../components/LootComponent.h"
#include "../components/StatComponent.h"

#include "../Helpers.h"

#include "../ecs/Engine.h"

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
  auto pickups = engine.getEntitiesWithComp(PICKUP_ID);

  // Keep track of which entities to delete. If we delete immediately, the entity* addresses get
  // corrupt because they are reshuffled inside of the engine
  std::vector<ecs::EntityID> pendingDelete;
  for (auto pickup: pickups) {
    auto pickupPosComp = static_cast<PositionComponent*>(pickup->getComp(POS_ID));

    if (helpers::distance(playerPosComp, pickupPosComp) <= 0.9) {
      applyPickup(player, pickup);
      pendingDelete.push_back(pickup->id());
    }
  }

  // Delete last, see comment above (maybe fix)
  for (auto id: pendingDelete) {
    engine.removeEntity(id);
  }
}