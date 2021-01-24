#include "InputSystem.h"

#include "../Orientation.h"
#include "../ecs/Engine.h"
#include "../ecs/Entity.h"
#include "../components/PlayerInputComponent.h"
#include "../components/CombatComponent.h"
#include "../components/PositionComponent.h"
#include "../components/SpriteComponent.h"
#include "../components/AbilityComponent.h"
#include "../components/StatComponent.h"

#include <iostream>

InputSystem::InputSystem(sf::RenderWindow& window)
  : _window(window)
{}

static bool isMovementKey(sf::Keyboard::Key key)
{
  return key == sf::Keyboard::D ||
         key == sf::Keyboard::S ||
         key == sf::Keyboard::A ||
         key == sf::Keyboard::W;
}

void InputSystem::run(ecs::Engine& engine)
{
  ecs::Entity* playerEntity = nullptr;
  auto entities = engine.getEntitiesWithComp(PLAYER_INPUT_ID);

  CombatComponent* playerCombatComp = nullptr;
  PositionComponent* playerPosComp = nullptr;
  SpriteComponent* playerSpriteComp = nullptr;
  AbilityComponent* playerAbComp = nullptr;
  StatComponent* playerStatComp = nullptr;

  if (!entities.empty()) {
    playerEntity = entities[0];
    playerCombatComp = static_cast<CombatComponent*>(playerEntity->getComp(COMBAT_ID));
    playerPosComp = static_cast<PositionComponent*>(playerEntity->getComp(POS_ID));
    playerSpriteComp = static_cast<SpriteComponent*>(playerEntity->getComp(SPRITE_ID));
    playerAbComp = static_cast<AbilityComponent*>(playerEntity->getComp(ABILITY_ID));
    playerStatComp = static_cast<StatComponent*>(playerEntity->getComp(STAT_ID));
  }

  sf::Event event;
  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      std::cout << "Closing!" << std::endl;
      _window.close();
    }
    else if (event.type == sf::Event::KeyPressed) {
      // If we are in combat
      if (playerEntity) {
        if (playerCombatComp->_inCombat) {
          if (playerCombatComp->_awaitingInput) {
            if (event.key.code == sf::Keyboard::Num1 ||
                event.key.code == sf::Keyboard::Num2 ||
                event.key.code == sf::Keyboard::Num3 ||
                event.key.code == sf::Keyboard::Num4 ||
                event.key.code == sf::Keyboard::Num5) {
              auto num = event.key.code - 27; // Num1 in the enum is 27, and the nextcoming numbers are adjacent
              if (playerAbComp->_abilities.size() > num) {
                // Make sure we can actually "afford" this ability in terms of AP
                if (playerAbComp->_abilities[num]._apCost <= playerStatComp->_ap &&
                    playerAbComp->_abilities[num]._type != AbilityComponent::Ability::Type::Move) {
                  playerCombatComp->_chosenAbility = num;
                }
              }
            }
            // Check if we're pressing a movement key and if we have a move ability in that case
            if (isMovementKey(event.key.code)) {
              for (std::size_t i = 0; i < playerAbComp->_abilities.size(); ++i) {
                auto& ability = playerAbComp->_abilities[i];
                if (ability._type == AbilityComponent::Ability::Type::Move && ability._apCost <= playerStatComp->_ap) {
                  playerCombatComp->_chosenAbility = (int)i;
                  ability._movedOrientation = keyToOrientation(event.key.code);
                }
              }
            }
          }
        }
        else {
          // not in combat TODO: Check bounds?
          if (event.key.code == sf::Keyboard::D) {
            playerPosComp->_x += 1.0;
          }
          else if (event.key.code == sf::Keyboard::A) {
            playerPosComp->_x -= 1.0;
          }
          else if (event.key.code == sf::Keyboard::W) {
            playerPosComp->_y -= 1.0;
          }
          else if (event.key.code == sf::Keyboard::S) {
            playerPosComp->_y += 1.0;
          }
          playerSpriteComp->_orientation = keyToOrientation(event.key.code);
        }
      }      
    }
  }
}