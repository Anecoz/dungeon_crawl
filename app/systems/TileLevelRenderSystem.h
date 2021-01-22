#pragma once

#include "../ecs/System.h"

#include <SFML/Graphics.hpp>

class TileLevelRenderSystem : public ecs::System
{
public:
  TileLevelRenderSystem(sf::RenderWindow& window);
  ~TileLevelRenderSystem() = default;

  void run(ecs::Engine& engine) override;
  unsigned priority() override { return 1; }

private:
  sf::RenderWindow& _window;
};