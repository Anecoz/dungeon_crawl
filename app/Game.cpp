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
#include "components/LootComponent.h"
#include "components/DoorComponent.h"

#include "systems/InputSystem.h"
#include "systems/SpriteRenderSystem.h"
#include "systems/TileLevelRenderSystem.h"
#include "systems/UIRenderSystem.h"
#include "systems/CombatSystem.h"
#include "systems/AISystem.h"
#include "systems/LootSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/LevelSystem.h"

#include <iostream>

#include <SFML/Window/Mouse.hpp>

Game::Game()
  : _window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "2D DC")
{}

bool Game::init()
{
  return fontcache::init();
}

std::unique_ptr<ecs::Entity> makePlayerEntity()
{
  auto entity = std::make_unique<ecs::Entity>();

  auto posComp = std::make_unique<PositionComponent>(1.0, 1.0);
  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/player-move.png"),
    WORLD_TO_PIXEL_X/64.0, WORLD_TO_PIXEL_Y/64.0, 0, 0, 64, 64);
  auto hpComp = std::make_unique<StatComponent>(100);
  auto combatComp = std::make_unique<CombatComponent>(CombatComponent::Faction::Player);
  auto inputComp = std::make_unique<PlayerInputComponent>();
  auto abilityComp = std::make_unique<AbilityComponent>();

  // Couple of abilities
  AbilityComponent::Ability ability1(RESOURCE_PATH + std::string("ability_textures/ability1.png"), 2, 1);
  AbilityComponent::Ability ability2(RESOURCE_PATH + std::string("ability_textures/ability2.png"), 4, 1);
  AbilityComponent::Ability ability3(RESOURCE_PATH + std::string("ability_textures/ability3.png"), 10, 3);
  AbilityComponent::Ability ability4(RESOURCE_PATH + std::string("ability_textures/ability4.png"), 50, 0);
  AbilityComponent::Ability ability5(RESOURCE_PATH + std::string("ability_textures/ability5.png"), 0, 1);
  ability5._type = AbilityComponent::Ability::Type::Move;

  abilityComp->_abilities.emplace_back(std::move(ability1));
  abilityComp->_abilities.emplace_back(std::move(ability2));
  abilityComp->_abilities.emplace_back(std::move(ability3));
  abilityComp->_abilities.emplace_back(std::move(ability4));
  abilityComp->_abilities.emplace_back(std::move(ability5));

  entity->addComp(std::move(hpComp));
  entity->addComp(std::move(spriteComp));
  entity->addComp(std::move(posComp));
  entity->addComp(std::move(combatComp));
  entity->addComp(std::move(inputComp));
  entity->addComp(std::move(abilityComp));

  return entity;
}

std::unique_ptr<ecs::Entity> makeMobEntity(double startX, double startY, bool loot = false)
{
  auto entity = std::make_unique<ecs::Entity>();

  auto posComp = std::make_unique<PositionComponent>(startX, startY);
  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/gargant-berserker-move.png"),
    WORLD_TO_PIXEL_X/64.0, WORLD_TO_PIXEL_Y/64.0, 0, 0, 64, 64);    
  auto hpComp = std::make_unique<StatComponent>(100);
  auto combatComp = std::make_unique<CombatComponent>(CombatComponent::Faction::Enemy);
  auto aiComp = std::make_unique<AIComponent>();
  auto abilityComp = std::make_unique<AbilityComponent>();
  AbilityComponent::Ability ability(RESOURCE_PATH + std::string("ability_textures/ability1.png"), 1, 2);
  AbilityComponent::Ability skipAbility(RESOURCE_PATH + std::string("ability_textures/ability4.png"), 0, 0);
  AbilityComponent::Ability moveAbility(RESOURCE_PATH + std::string("ability_textures/ability5.png"), 0, 2);
  moveAbility._type = AbilityComponent::Ability::Type::Move;
  skipAbility._type = AbilityComponent::Ability::Type::Pacifist;

  abilityComp->_abilities.emplace_back(std::move(moveAbility));
  abilityComp->_abilities.emplace_back(std::move(ability));
  abilityComp->_abilities.emplace_back(std::move(skipAbility));

  if (loot) {
    auto lootComp = std::make_unique<LootComponent>();
    lootComp->_type = LootComponent::Type::Health;
    lootComp->_hp = 10;
    entity->addComp(std::move(lootComp));
  }

  entity->addComp(std::move(hpComp));
  entity->addComp(std::move(spriteComp));
  entity->addComp(std::move(posComp));
  entity->addComp(std::move(combatComp));
  entity->addComp(std::move(abilityComp));
  entity->addComp(std::move(aiComp));
  return entity;
}

std::unique_ptr<ecs::Entity> makeLevelEntity()
{
  auto entity = std::make_unique<ecs::Entity>();

  auto tileComp = std::make_unique<TiledSpritesComponent>(
    RESOURCE_PATH + std::string("spritesheets/mainlevbuild.png"),
    10, WORLD_TO_PIXEL_X/32.0, WORLD_TO_PIXEL_Y/32.0, 736, 272, 32, 32);

  auto posComp = std::make_unique<PositionComponent>(0.0, 0.0);

  entity->addComp(std::move(tileComp));
  entity->addComp(std::move(posComp));

  return entity;
}

std::unique_ptr<ecs::Entity> makeDoorEntity()
{
  auto entity = std::make_unique<ecs::Entity>();

  auto spriteComp = std::make_unique<SpriteComponent>(
    RESOURCE_PATH + std::string("spritesheets/mainlevbuild.png"),
    WORLD_TO_PIXEL_X/40.0, WORLD_TO_PIXEL_Y/49.0,
    797, 55,
    40, 49);
  auto posComp = std::make_unique<PositionComponent>(LEVEL_SIZE_X - 1.0, 5.0);
  auto doorComp = std::make_unique<DoorComponent>();
  
  entity->addComp(std::move(spriteComp));
  entity->addComp(std::move(posComp));
  entity->addComp(std::move(doorComp));

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
  ecsEngine.addSystem(std::make_unique<LootSystem>());
  ecsEngine.addSystem(std::make_unique<CollisionSystem>());
  ecsEngine.addSystem(std::make_unique<LevelSystem>());

  ecsEngine.addEntity(makePlayerEntity());
  ecsEngine.addEntity(makeLevelEntity());
  ecsEngine.addEntity(makeMobEntity(5.0, 3.0, true));
  ecsEngine.addEntity(makeMobEntity(4.0, 4.0, true));
  ecsEngine.addEntity(makeMobEntity(4.0, 3.0, true));
  ecsEngine.addEntity(makeDoorEntity());

  sf::Clock clock;
  while (_window.isOpen()) {
    _window.clear(sf::Color::Black);
    ecsEngine.runOnce();

    // Update
    //sf::Time elapsed = clock.restart();

    _window.display();
  }
}