#include "Orientation.h"

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
  return Orientation::North;
}