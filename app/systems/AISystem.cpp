#include "AISystem.h"

#include "../components/ComponentIds.h"
#include "../components/CombatComponent.h"
#include "../components/AbilityComponent.h"
#include "../components/StatComponent.h"
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
    auto statComp = static_cast<StatComponent*>(entity->getComp(STAT_ID));
    if (!combatComp || !statComp) continue;

    if (combatComp->_awaitingInput) {
      auto abilityComp = static_cast<AbilityComponent*>(entity->getComp(ABILITY_ID));
      if (!abilityComp) {
        std::cout << "AI component of entity " << std::to_string(entity->id()) << " can't find an ability component, he can't fight :(" << std::endl;
        continue;
      }
      
      // Choose a random ability, but first filter which ones we can afford
      std::vector<std::size_t> affordableAbilities;
      for (std::size_t i = 0; i < abilityComp->_abilities.size(); ++i) {
        if (statComp->_ap >= abilityComp->_abilities[i]._apCost) {
          affordableAbilities.push_back(i);
        }
      }
      if (!affordableAbilities.empty()) {
        auto chosen = rand() % affordableAbilities.size();
        combatComp->_chosenAbility = static_cast<int>(affordableAbilities[chosen]);
      }      
    }
  }
}