#pragma once

#include "../ecs/System.h"

#include <SFML/Graphics.hpp>

class UIRenderSystem : public ecs::System
{
public:
  UIRenderSystem(sf::RenderWindow& window);
  ~UIRenderSystem() = default;

  void run(ecs::Engine& engine) override;

private:
  sf::RenderWindow& _window;
};