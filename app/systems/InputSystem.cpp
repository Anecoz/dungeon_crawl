#include "InputSystem.h"

#include <iostream>

InputSystem::InputSystem(sf::RenderWindow& window)
  : _window(window)
{}

void InputSystem::run(ecs::Engine& engine)
{
  sf::Event event;
  while (_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      std::cout << "Closing!" << std::endl;
      _window.close();
    }
  }
}