#include "Game.h"

#include "FontCache.h"

#include "ecs/Engine.h"

#include "Constants.h"

#include "components/PositionComponent.h"
#include "components/SpriteComponent.h"
#include "components/TiledSpritesComponent.h"
#include "components/StatComponent.h"
#include "components/CombatComponent.h"
#include "components/PlayerInputComponent.h"
#include "components/AbilityComponent.h"
#include "components/AIComponent.h"

#include "systems/InputSystem.h"
#include "systems/SpriteRenderSystem.h"
#include "systems/TileLevelRenderSystem.h"
#include "systems/UIRenderSystem.h"
#include "systems/CombatSystem.h"
#include "systems/AISystem.h"

#include "Level.h"

#include <iostream>

#include <SFML/Window/Mouse.hpp>

Game::Game()
  : _window(sf::VideoMode(1280, 720), "2D DC")
{}

bool Game::init()
{
  return fontcache::init();
}

ecs::Entity makePlayerEntity()
{
  ecs::Entity entity;

  auto posComp = std::make_unique<PositionComponent>(1.0, 1.0);
  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/player-move.png"),
    2.0, 0, 0, 64, 64);
  auto hpComp = std::make_unique<StatComponent>(100);
  auto combatComp = std::make_unique<CombatComponent>(CombatComponent::Faction::Player);
  auto inputComp = std::make_unique<PlayerInputComponent>();
  auto abilityComp = std::make_unique<AbilityComponent>();
  AbilityComponent::Ability ability;
  ability._damage = 2;
  abilityComp->_abilities.emplace_back(std::move(ability));

  entity.addComp(std::move(hpComp));
  entity.addComp(std::move(spriteComp));
  entity.addComp(std::move(posComp));
  entity.addComp(std::move(combatComp));
  entity.addComp(std::move(inputComp));
  entity.addComp(std::move(abilityComp));
  return entity;
}

ecs::Entity makeMobEntity()
{
  ecs::Entity entity;

  auto posComp = std::make_unique<PositionComponent>(5.0, 3.0);
  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/gargant-berserker-move.png"),
    2.0, 0, 0, 64, 64);    
  auto hpComp = std::make_unique<StatComponent>(100);
  auto combatComp = std::make_unique<CombatComponent>(CombatComponent::Faction::Enemy);
  auto aiComp = std::make_unique<AIComponent>();
  auto abilityComp = std::make_unique<AbilityComponent>();
  AbilityComponent::Ability ability;
  ability._damage = 1;
  abilityComp->_abilities.emplace_back(std::move(ability));

  entity.addComp(std::move(hpComp));
  entity.addComp(std::move(spriteComp));
  entity.addComp(std::move(posComp));
  entity.addComp(std::move(combatComp));
  entity.addComp(std::move(abilityComp));
  entity.addComp(std::move(aiComp));
  return entity;
}

ecs::Entity makeLevelEntity()
{
  ecs::Entity entity;

  auto tileComp = std::make_unique<TiledSpritesComponent>(
    RESOURCE_PATH + std::string("spritesheets/mainlevbuild.png"),
    10, 4.0f, 736, 272, 32, 32);

  auto posComp = std::make_unique<PositionComponent>(0.0, 0.0);

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
  ecsEngine.addSystem(std::make_unique<UIRenderSystem>(_window));
  ecsEngine.addSystem(std::make_unique<CombatSystem>());
  ecsEngine.addSystem(std::make_unique<AISystem>());

  ecsEngine.addEntity(makePlayerEntity());
  ecsEngine.addEntity(makeLevelEntity());
  ecsEngine.addEntity(makeMobEntity());

  sf::Clock clock;
  while (_window.isOpen()) {
    _window.clear(sf::Color::Black);
    ecsEngine.runOnce();

    // Update
    //sf::Time elapsed = clock.restart();

    _window.display();
  }
}