#include "TileLevelRenderSystem.h"

#include "../Constants.h"
#include "../ecs/Engine.h"
#include "../ecs/Entity.h"
#include "../components/ComponentIds.h"
#include "../components/PositionComponent.h"
#include "../components/TiledSpritesComponent.h"

TileLevelRenderSystem::TileLevelRenderSystem(sf::RenderWindow& window)
  : _window(window)
{}

void TileLevelRenderSystem::run(ecs::Engine& engine)
{
  auto entities = engine.getEntitiesWithComp(TILED_SPRITE_ID);
  for (auto& entity: entities) {
    auto spriteComp = static_cast<TiledSpritesComponent*>(entity->getComp(TILED_SPRITE_ID));
    auto posComp = static_cast<PositionComponent*>(entity->getComp(POS_ID));

    if (!spriteComp || !posComp) continue;

    for (auto& spriteTile: spriteComp->_sprites) {
      spriteTile._sprite.setPosition(
        static_cast<float>(WORLD_TO_PIXEL * posComp->_x + spriteTile._x),
        static_cast<float>(WORLD_TO_PIXEL * posComp->_y + spriteTile._y));
      _window.draw(spriteTile._sprite);
    }
  }
}