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

static void makeNextTurn(ecs::Entity* entity)
{
  static_cast<StatComponent*>(entity->getComp(STAT_ID))->_ap++;
  static_cast<CombatComponent*>(entity->getComp(COMBAT_ID))->_awaitingInput = true;
}

static void ensureTargets(const std::vector<ecs::EntityID>& fighters, ecs::Engine& engine)
{
  for (auto id: fighters) {
    auto entity = engine.getEntityById(id);
    auto combatComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
    
    // Is the target in the fighters list?
    auto findIt = std::find(fighters.begin(), fighters.end(), combatComp->_target);
    if (findIt == fighters.end()) {
      // Not in list, so find a new target
      for (auto potentialTarget: fighters) {
        if (potentialTarget == id) continue;
        auto potTargetEnt = engine.getEntityById(potentialTarget);
        auto tarCombatComp = static_cast<CombatComponent*>(potTargetEnt->getComp(COMBAT_ID));
        if (tarCombatComp->_faction != combatComp->_faction) {
          combatComp->_target = potentialTarget;
          break;
        }
      }
    }
  }
}

void CombatSystem::run(ecs::Engine& engine)
{
  // Deal with ongoing combat
  ecs::EntityID thisRoundFighter;
  if (_ongoingFight) {
    // Figure out who's turn it is
    for (auto it = _fighters.begin(); it != _fighters.end(); ++it) {
      auto entity = engine.getEntityById(*it);
      auto combatComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
      if (combatComp->_awaitingInput && combatComp->_chosenAbility != -1) {
        // Apply the chosen ability. TODO: Deal with things like moving and fleeing? Also abilities?
        auto abilityComp = static_cast<AbilityComponent*>(entity->getComp(ABILITY_ID));        
        if (!abilityComp) {
          std::cout << "Entity with id " << std::to_string(entity->id()) << " tried to choose an ability, but has no ability component!" << std::endl;
          continue;
        }

        std::cout << "Entity with id " << std::to_string(entity->id()) << " chose ability " << std::to_string(combatComp->_chosenAbility) << "!" << std::endl;
        applyAbility(entity, engine.getEntityById(combatComp->_target), abilityComp->_abilities[combatComp->_chosenAbility]);

        thisRoundFighter = *it;

        combatComp->_awaitingInput = false;
        combatComp->_chosenAbility = -1;

        // Choose the next entity who's turn it is
        ecs::EntityID next;
        if (it + 1 != _fighters.end()) {
          next = _fighters[it + 1 - _fighters.begin()];
        }
        else {
          // First one, wrap around
          next = _fighters[0];
        }
        std::cout << "It is now entity with id " << std::to_string(next) << "'s turn!" << std::endl;
        // Let the comp know it's awaiting input and also give 1 more AP
        makeNextTurn(engine.getEntityById(next));
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
          std::cout << "Fight is over!" << std::endl;
          auto entity = engine.getEntityById(entityID);
          auto combComp =  static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
          combComp->_inCombat = false;
          combComp->_awaitingInput = false;
          _ongoingFight = false;
          _fighters.clear();
        }
      }
      else {
        // Fight still going on, make sure that it's still someones turn (he might have died)
        bool someonesTurn = false;
        for (auto id: _fighters) {
          auto entity = engine.getEntityById(id);
          auto combComp = static_cast<CombatComponent*>(entity->getComp(COMBAT_ID));
          if (combComp->_awaitingInput) {
            someonesTurn = true;
            break;
          }
        }
        if (!someonesTurn) {
          // Make the first entity in list that didn't fight this round the next turn
          for (auto id: _fighters) {
            if (id != thisRoundFighter) {
              makeNextTurn(engine.getEntityById(id));
              break;
            }
          }
        }
        // Also ensure everyone has a target (that is alive and so on)
        ensureTargets(_fighters, engine);
      }
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