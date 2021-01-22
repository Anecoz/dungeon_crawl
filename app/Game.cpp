#include "Game.h"

#include "ecs/Engine.h"
#include "systems/InputSystem.h"
#include "Level.h"

#include <iostream>

#include <SFML/Window/Mouse.hpp>

Game::Game()
  : _window(sf::VideoMode(1280, 720), "2D DC")
{}

bool Game::init()
{
  return true;
}

void Game::run()
{
  ecs::Engine ecsEngine;

  Level level;
  if (!level.init()) {
    std::cerr << "Could not load level!" << std::endl;
  }

  ecsEngine.addSystem(std::make_unique<InputSystem>(_window));

  sf::Clock clock;
  while (_window.isOpen()) {
    ecsEngine.runOnce();

    // Update
    sf::Time elapsed = clock.restart();

    // Render
    _window.clear(sf::Color::Black);

    level.render(_window);

    _window.display();
  }
}