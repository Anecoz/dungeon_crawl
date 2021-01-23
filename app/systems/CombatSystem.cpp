#include "CombatSystem.h"

#include "../components/CombatComponent.h"
#include "../components/AbilityComponent.h"
#include "../components/HealthComponent.h"
#include "../ecs/Engine.h"

#include <string>
#include <iostream>

void CombatSystem::run(ecs::Engine& engine)
{
  // Deal with ongoing combat
  if (_ongoingFight) {
    // Retrieve our figher entities
    std::vector<ecs::Entity*> entities;
    for (auto id: _fighters) {
      entities.push_back(engine.getEntityById(id));
    }

    // Figure out who's turn it is
    for (auto it = entities.begin(); it != entities.end(); ++it) {
      auto combatComp = static_cast<CombatComponent*>((*it)->getComp(COMBAT_ID));
      if (combatComp->_awaitingInput && combatComp->_chosenAbility != -1) {
        // Apply the chosen ability. TODO: Deal with things like moving and fleeing? Also abilities?
        auto abilityComp = static_cast<AbilityComponent*>((*it)->getComp(ABILITY_ID));
        if (!abilityComp) {
          std::cout << "Entity with id " << std::to_string((*it)->id()) << " tried to choose an ability, but has no ability component!" << std::endl;
          continue;
        }
        auto target = engine.getEntityById(combatComp->_target);
        auto targetHpComp = static_cast<HealthComponent*>(target->getComp(HEALTH_ID));

        // TODO: Remove entitiy if it dies (from engine and from the list of fighters etc)
        std::cout << "Entity with id " << std::to_string((*it)->id()) << " chose ability " << std::to_string(combatComp->_chosenAbility) << "!" << std::endl;
        targetHpComp->_health -= abilityComp->_abilities[combatComp->_chosenAbility]._damage;

        combatComp->_awaitingInput = false;
        combatComp->_chosenAbility = -1;

        // Choose the next entity who's turn it is
        ecs::Entity* next = nullptr;
        if (it + 1 != entities.end()) {
          next = entities[it + 1 - entities.begin()];
        }
        else {
          // First one, wrap around
          next = entities[0];
        }
        std::cout << "It is now entity with id " << std::to_string(next->id()) << "'s turn!" << std::endl;
        static_cast<CombatComponent*>(next->getComp(COMBAT_ID))->_awaitingInput = true;
        break;
      }
    }
  }
  else {
    // We are not in an ongoing fight, so check if we should start one (ง'̀-'́)ง
    auto entities = engine.getEntitiesWithComp(COMBAT_ID);
    for (auto entity: entities) {
      auto combatComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
      if (combatComp->_faction == CombatComponent::Faction::Player) {
        // Look for enemies nearby and start a fight
        std::vector<ecs::Entity*> enemies;
        for (auto ent2: entities) {
          // Don't mind ourselves
          if (ent2->id() == entity->id()) continue;
          auto enemyCombComp = static_cast<CombatComponent*>(ent2->getComp(COMBAT_ID));
          if (enemyCombComp->_faction == CombatComponent::Faction::Enemy) {
            enemies.push_back(ent2);
            enemyCombComp->_inCombat = true;
            enemyCombComp->_awaitingInput = false;
            enemyCombComp->_target = entity->id();
          }
        }
        // Put the player in combat aswell
        if (!enemies.empty()) {
          combatComp->_inCombat = true;
          combatComp->_awaitingInput = true;
          combatComp->_target = enemies[0]->id();

          // Update the system state
          _fighters.clear();
          _ongoingFight = true;
          _fighters.push_back(entity->id());
          for (auto enemy: enemies) {
            _fighters.push_back(enemy->id());
          }

          // Do some output logging
          std::cout << "These entities are now in combat: ";
          for (auto fighter: _fighters) {
            std::cout << std::to_string(fighter) << ", ";
          }
          std::cout << std::endl;
        }
      }
    }
  }
}