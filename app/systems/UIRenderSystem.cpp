#include "UIRenderSystem.h"

#include "../Constants.h"
#include "../FontCache.h"
#include "../ecs/Engine.h"
#include "../components/ComponentIds.h"
#include "../components/StatComponent.h"
#include "../components/PositionComponent.h"
#include "../components/PlayerInputComponent.h"
#include "../components/AbilityComponent.h"

#include <iostream>

UIRenderSystem::UIRenderSystem(sf::RenderWindow& window)
  : _window(window)
{}

void UIRenderSystem::run(ecs::Engine& engine)
{
  auto entities = engine.getEntitiesWithComp(STAT_ID);
  for (auto& entity: entities) {
    auto hpComp = static_cast<StatComponent*>(entity->getComp(STAT_ID));
    auto posComp = static_cast<PositionComponent*>(entity->getComp(POS_ID));

    if (!hpComp || !posComp) return;

    // Render the health text of the entity
    sf::Text healthText;    
    healthText.setFont(fontcache::font());
    healthText.setCharacterSize(24);
    healthText.setString(std::to_string(hpComp->_health) + std::string("/") + std::to_string(hpComp->_maxHealth));
    auto bounds = healthText.getLocalBounds();
    healthText.setPosition(static_cast<float>((posComp->_x * WORLD_TO_PIXEL) - bounds.width/2.0f), static_cast<float>((posComp->_y - 0.7f) * WORLD_TO_PIXEL));
    _window.draw(healthText);
  }

  // Render player's abilities
  auto playerEntities = engine.getEntitiesWithComp(PLAYER_INPUT_ID);
  if (!playerEntities.empty()) {
    auto player = playerEntities[0];
    auto abilityComp = static_cast<AbilityComponent*>(player->getComp(ABILITY_ID));
    if (abilityComp && !abilityComp->_abilities.empty()) {
      // Figure out where to start drawing abilities
      auto abilityWidthTot = abilityComp->_abilities.size() * ABILITY_TEX_SIZE;
      auto startX = SCREEN_WIDTH/2 - abilityWidthTot/2;
      auto startY = SCREEN_HEIGHT - ABILITY_TEX_SIZE;
      for (std::size_t i = 0; i < abilityComp->_abilities.size(); ++i) {
        float x = (float)startX + (float)(ABILITY_TEX_SIZE*i);
        float y = (float)startY;
        abilityComp->_abilities[i]._sprite.setPosition(x, y);
        _window.draw(abilityComp->_abilities[i]._sprite);

        // Also have text showing the hotkeys
        sf::Text hotkeyText;
        hotkeyText.setFont(fontcache::font());
        hotkeyText.setCharacterSize(20);
        hotkeyText.setString(std::to_string(i + 1));
        auto bounds = hotkeyText.getLocalBounds();
        hotkeyText.setPosition(x + (float)ABILITY_TEX_SIZE/2.0f - bounds.width/2.0f, y - 32.0f);
        _window.draw(hotkeyText);
      }
    }
  }
}