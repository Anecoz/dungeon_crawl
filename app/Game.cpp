#include "Game.h"

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

void Game::run()
{
  Level level;
  if (!level.init()) {
    std::cerr << "Could not load level!" << std::endl;
  }

  sf::Clock clock;
  while (_window.isOpen()) {
    // Check and act on input first
    sf::Event event;
    while (_window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        _window.close();
      }
    }

    // Update
    sf::Time elapsed = clock.restart();

    // Render
    _window.clear(sf::Color::Black);

    level.render(_window);

    _window.display();
  }
}