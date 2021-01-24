#include "AISystem.h"

#include "../Helpers.h"
#include "../components/ComponentIds.h"
#include "../components/CombatComponent.h"
#include "../components/PositionComponent.h"
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
      
      // Pre-check if we have a move ability
      int moveIdx = -1;
      bool moveAffordable = false;
      for (std::size_t i = 0; i < abilityComp->_abilities.size(); ++i) {
        if (abilityComp->_abilities[i]._type == AbilityComponent::Ability::Type::Move) {
          moveIdx = static_cast<int>(i);
          break;
        }
      }

      // Choose a random ability, but first filter which ones we can afford
      std::vector<std::size_t> affordableAbilities;
      for (std::size_t i = 0; i < abilityComp->_abilities.size(); ++i) {
        if (statComp->_ap >= abilityComp->_abilities[i]._apCost) {
          affordableAbilities.push_back(i);
          if ((int)i == moveIdx) {
            moveAffordable = true;
          }
        }
      }
      if (!affordableAbilities.empty()) {
        auto target = engine.getEntityById(combatComp->_target);
        auto targetPosComp = static_cast<PositionComponent*>(target->getComp(POS_ID));
        auto posComp = static_cast<PositionComponent*>(entity->getComp(POS_ID));

        double distance = helpers::distance(posComp, targetPosComp);

        bool moved = false;
        // If we are far from the target, attempt to move if we have a move ability
        if (moveIdx != -1 && moveAffordable) {          
          if (distance > 1.5) {
            combatComp->_chosenAbility = moveIdx;
            abilityComp->_abilities[moveIdx]._movedOrientation = helpers::direction(posComp, targetPosComp);
            moved = true;
          }
        }

        if (!moved) {
          // If we are in range, try to do damage
          if (distance <= 1.5) {
            for (auto idx: affordableAbilities) {
              if (abilityComp->_abilities[idx]._type == AbilityComponent::Ability::Type::Damage) {
                combatComp->_chosenAbility = static_cast<int>(idx);
                return;
              }
            }
          }
          // If we didn't move and didn't attack, try to skip?
          for (auto idx: affordableAbilities) {
            if (abilityComp->_abilities[idx]._type == AbilityComponent::Ability::Type::Pacifist) {
              combatComp->_chosenAbility = static_cast<int>(idx);
              return;
            }
          }

          // Last resort just do something random I guess
          auto chosen = rand() % affordableAbilities.size();
          combatComp->_chosenAbility = static_cast<int>(affordableAbilities[chosen]);  
        }   
      }      
    }
  }
}