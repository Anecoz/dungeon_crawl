#pragma once

#include "../ecs/System.h"

#include <SFML/Graphics.hpp>

class SpriteRenderSystem : public ecs::System
{
public:
  SpriteRenderSystem(sf::RenderWindow& window);
  ~SpriteRenderSystem() = default;

  void run(ecs::Engine& engine) override;
  unsigned priority() override { return 2; }

private:
  sf::RenderWindow& _window;
};