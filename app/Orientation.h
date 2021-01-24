#pragma once

#include <SFML/Window/Keyboard.hpp>

enum class Orientation
{
  North,
  NorthEast,
  East,
  SouthEast,
  South,
  SouthWest,
  West,
  NorthWest
};

Orientation keyToOrientation(sf::Keyboard::Key key)
{
  switch (key) {
    case sf::Keyboard::D:
      return Orientation::East;
    case sf::Keyboard::S:
      return Orientation::South;
    case sf::Keyboard::A:
      return Orientation::West;
    case sf::Keyboard::W:
      return Orientation::North;
  }
}