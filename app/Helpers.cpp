#include "Helpers.h"

#include "components/PositionComponent.h"

#include <cmath>

#define PI 3.14159265359

namespace helpers {

double distance(PositionComponent* comp1, PositionComponent* comp2)
{
  return std::sqrt(std::pow(comp2->_x - comp1->_x, 2) + std::pow(comp2->_y - comp1->_y, 2));
}

// From comp1 to comp2
Orientation direction(PositionComponent* comp1, PositionComponent* comp2)
{
  // Get vector from 1 to 2
  double vecX = comp2->_x - comp1->_x;
  double vecY = comp2->_y - comp1->_y;

  // Up vector
  double upX = 0.0;
  double upY = -1.0;

  double angle = std::acos(
    (vecX * upX + vecY * upY) /
    (std::sqrt(std::pow(vecX, 2) + std::pow(vecY, 2)) * std::sqrt(std::pow(upX, 2) + std::pow(upY, 2))));

  angle = angle * 180.0 / PI;

  // if comp2 is left of comp1, make sure we're on [0, 360]
  if (comp2->_x < comp1->_x) {
    angle = 360.0 - angle;
  }

  if ((angle <= 45.0 && angle >= 0.0) || (angle >= 315.0 && angle <= 360.0)) {
    return Orientation::North;
  }
  else if (angle >= 45.0 && angle <= 135.0) {
    return Orientation::East;
  }
  else if (angle >= 135.0 && angle <= 225.0) {
    return Orientation::South;
  }
  else if (angle >= 225.0 && angle <= 315.0) {
    return Orientation::West;
  }

  return Orientation::North;
}

}