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

Orientation keyToOrientation(sf::Keyboard::Key key);