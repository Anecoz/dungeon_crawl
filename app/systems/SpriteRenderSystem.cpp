#include "SpriteRenderSystem.h"

#include "../ecs/Engine.h"
#include "../ecs/Entity.h"
#include "../components/ComponentIds.h"
#include "../components/PositionComponent.h"
#include "../components/SpriteComponent.h"

SpriteRenderSystem::SpriteRenderSystem(sf::RenderWindow& window)
  : _window(window)
{}

void SpriteRenderSystem::run(ecs::Engine& engine)
{
  auto entities = engine.getEntitiesWithComp(SPRITE_ID);
  for (auto& entity: entities) {
    auto spriteComp = static_cast<SpriteComponent*>(entity->getComp(SPRITE_ID));
    auto posComp = static_cast<PositionComponent*>(entity->getComp(POS_ID));

    if (!spriteComp || !posComp) continue;

    spriteComp->_sprite.setPosition(static_cast<float>(posComp->_x), static_cast<float>(posComp->_y));
    _window.draw(spriteComp->_sprite);
  }
}