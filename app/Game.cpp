#include "Game.h"

#include "ecs/Engine.h"
#include "ecs/Entity.h"

#include "components/PositionComponent.h"
#include "components/SpriteComponent.h"
#include "components/TiledSpritesComponent.h"

#include "systems/InputSystem.h"
#include "systems/SpriteRenderSystem.h"
#include "systems/TileLevelRenderSystem.h"

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

ecs::Entity makePlayerEntity()
{
  ecs::Entity entity;

  auto posComp = std::make_unique<PositionComponent>(50.0, 50.0);

  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/player-move.png"),
    2.0,
    0, 0,
    64, 64
  );

  entity.addComp(std::move(spriteComp));
  entity.addComp(std::move(posComp));
  return entity;
}

ecs::Entity makeLevelEntity()
{
  ecs::Entity entity;

  auto tileComp = std::make_unique<TiledSpritesComponent>(
    RESOURCE_PATH + std::string("spritesheets/mainlevbuild.png"),
    5, 4.0f, 736, 272, 32, 32);

  auto posComp = std::make_unique<PositionComponent>(100.0, 0.0);

  entity.addComp(std::move(tileComp));
  entity.addComp(std::move(posComp));

  return entity;
}

void Game::run()
{
  ecs::Engine ecsEngine;

  ecsEngine.addSystem(std::make_unique<InputSystem>(_window));
  ecsEngine.addSystem(std::make_unique<SpriteRenderSystem>(_window));
  ecsEngine.addSystem(std::make_unique<TileLevelRenderSystem>(_window));

  ecsEngine.addEntity(makePlayerEntity());
  ecsEngine.addEntity(makeLevelEntity());

  sf::Clock clock;
  while (_window.isOpen()) {
    _window.clear(sf::Color::Black);
    ecsEngine.runOnce();

    // Update
    //sf::Time elapsed = clock.restart();

    _window.display();
  }
}