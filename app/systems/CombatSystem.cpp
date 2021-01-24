#include "CombatSystem.h"

#include "../components/CombatComponent.h"
#include "../components/AbilityComponent.h"
#include "../components/StatComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SpriteComponent.h"
#include "../ecs/Engine.h"

#include <algorithm>
#include <string>
#include <iostream>

void applyAbility(ecs::Entity* self, ecs::Entity* target, AbilityComponent::Ability& ability)
{
  switch (ability._type) {
    case AbilityComponent::Ability::Type::Damage:
    {
      auto targetHpComp = static_cast<StatComponent*>(target->getComp(STAT_ID));
      targetHpComp->_health -= ability._damage;
      break;
    }
    case AbilityComponent::Ability::Type::Move:
    {
      auto posComp = static_cast<PositionComponent*>(self->getComp(POS_ID));
      auto spriteComp = static_cast<SpriteComponent*>(self->getComp(SPRITE_ID));
      switch (ability._movedOrientation) {
        case Orientation::North:
          posComp->_y -= 1.0f;
          break;
        case Orientation::NorthEast:
          posComp->_y -= 1.0f;
          posComp->_x += 1.0f;
          break;
        case Orientation::East:
          posComp->_x += 1.0f;
          break;
        case Orientation::SouthEast:
          posComp->_y += 1.0f;
          posComp->_x += 1.0f;
          break;
        case Orientation::South:
          posComp->_y += 1.0f;
          break;
        case Orientation::SouthWest:
          posComp->_y += 1.0f;
          posComp->_x -= 1.0f;
          break;
        case Orientation::West:
          posComp->_x -= 1.0f;
          break;
        case Orientation::NorthWest:
          posComp->_y -= 1.0f;
          posComp->_x -= 1.0f;
          break;
      }
      spriteComp->_orientation = ability._movedOrientation;
      break;
    }
  }

  // Make the ability cost
  auto statComp = static_cast<StatComponent*>(self->getComp(STAT_ID));
  statComp->_ap -= ability._apCost;
}

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

        std::cout << "Entity with id " << std::to_string((*it)->id()) << " chose ability " << std::to_string(combatComp->_chosenAbility) << "!" << std::endl;
        applyAbility(*it, engine.getEntityById(combatComp->_target), abilityComp->_abilities[combatComp->_chosenAbility]);

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
        // Let the comp know it's awaiting input and also give 1 more AP
        static_cast<StatComponent*>(next->getComp(STAT_ID))->_ap++;
        static_cast<CombatComponent*>(next->getComp(COMBAT_ID))->_awaitingInput = true;
        break;
      }
    }

    // Do a post-pass and check if anyone died
    for (auto it = _fighters.begin(); it != _fighters.end();) {
      auto entity = engine.getEntityById(*it);
      auto hpComp = static_cast<StatComponent*>(entity->getComp(STAT_ID));
      if (hpComp->_health <= 0) {
        engine.removeEntity(*it);
        it = _fighters.erase(it);
      }
      else {
        ++it;
      }
    }

    // Do we after removal etc. still have a fight?
    if (_fighters.size() > 0) {
      // Check so that there are unique factions involved, else they won
      std::size_t numPlayers = 0;
      std::size_t numEnemies = 0;
      for (auto entityID: _fighters) {
        auto entity = engine.getEntityById(entityID);
        auto combComp =  static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
        if (combComp->_faction == CombatComponent::Faction::Player) numPlayers++;
        else numEnemies++;
      }

      if (!(numPlayers > 0 && numEnemies > 0)) {
        for (auto entityID: _fighters) {
          auto entity = engine.getEntityById(entityID);
          auto combComp =  static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
          combComp->_inCombat = false;
          combComp->_awaitingInput = false;
          _ongoingFight = false;
          _fighters.clear();
        }
      }
    }
    else {
      // No one left
      _ongoingFight = false;
    }

    if (!_ongoingFight) {
      std::cout << "Fight is over!" << std::endl;
    }
  }
  else {
    // We are not in an ongoing fight, so check if we should start one (ง'̀-'́)ง
    auto entities = engine.getEntitiesWithComp(COMBAT_ID);
    for (auto entity: entities) {
      auto combatComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
      auto statComp = static_cast<StatComponent*>(entity->getComp(STAT_ID));
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
          statComp->_ap++;
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