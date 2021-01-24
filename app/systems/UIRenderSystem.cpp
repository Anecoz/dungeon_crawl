#include "UIRenderSystem.h"

#include "../Constants.h"
#include "../FontCache.h"
#include "../ecs/Engine.h"
#include "../components/ComponentIds.h"
#include "../components/StatComponent.h"
#include "../components/PositionComponent.h"

#include <iostream>

UIRenderSystem::UIRenderSystem(sf::RenderWindow& window)
  : _window(window)
{}

void UIRenderSystem::run(ecs::Engine& engine)
{
  auto entities = engine.getEntitiesWithComp(HEALTH_ID);
  for (auto& entity: entities) {
    auto hpComp = static_cast<HealthComponent*>(entity->getComp(HEALTH_ID));
    auto posComp = static_cast<PositionComponent*>(entity->getComp(POS_ID));

    if (!hpComp || !posComp) return;

    sf::Text healthText;
    healthText.setPosition(static_cast<float>(posComp->_x * WORLD_TO_PIXEL), static_cast<float>((posComp->_y - 0.25) * WORLD_TO_PIXEL));
    healthText.setFont(fontcache::font());
    healthText.setCharacterSize(24);
    healthText.setString(std::to_string(hpComp->_health) + std::string("/") + std::to_string(hpComp->_maxHealth));
    _window.draw(healthText);
  }
}