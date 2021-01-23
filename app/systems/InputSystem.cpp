#include "InputSystem.h"

#include "../ecs/Engine.h"
#include "../ecs/Entity.h"
#include "../components/PlayerInputComponent.h"
#include "../components/CombatComponent.h"

#include <iostream>

InputSystem::InputSystem(sf::RenderWindow& window)
  : _window(window)
{}

void InputSystem::run(ecs::Engine& engine)
{
  ecs::Entity* playerEntity = nullptr;
  auto entities = engine.getEntitiesWithComp(PLAYER_INPUT_ID);
  CombatComponent* playerCombatComp = nullptr;
  if (!entities.empty()) {
    playerEntity = entities[0];
    playerCombatComp = static_cast<CombatComponent*>(playerEntity->getComp(COMBAT_ID));
  }

  sf::Event event;
  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      std::cout << "Closing!" << std::endl;
      _window.close();
    }
    else if (event.type == sf::Event::KeyPressed) {
      if (playerEntity && playerCombatComp->_awaitingInput) {
        if (event.key.code == sf::Keyboard::Num1) {
          playerCombatComp->_chosenAbility = 0;
        }
      }
    }
  }
}