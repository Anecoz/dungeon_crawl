#include "AISystem.h"

#include "../components/ComponentIds.h"
#include "../components/CombatComponent.h"
#include "../components/AbilityComponent.h"
#include "../ecs/Engine.h"

#include <cstdlib> // for rand
#include <iostream>
#include <string>

void AISystem::run(ecs::Engine& engine)
{
  auto entities = engine.getEntitiesWithComp(AI_ID);
  for (auto entity: entities) {
    // Check if they have a waiting input for an ongoing combat thing
    auto combatComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
    if (!combatComp) continue;

    if (combatComp->_awaitingInput) {
      auto abilityComp = static_cast<AbilityComponent*>(entity->getComp(ABILITY_ID));
      if (!abilityComp) {
        std::cout << "AI component of entity " << std::to_string(entity->id()) << " can't find an ability component, he can't fight :(" << std::endl;
        continue;
      }
      
      // Choose a random ability
      combatComp->_chosenAbility = rand() % abilityComp->_abilities.size();
    }
  }
}