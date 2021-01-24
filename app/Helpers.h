#pragma once

#include "Orientation.h"

class PositionComponent;

namespace helpers {

double distance(PositionComponent* comp1, PositionComponent* comp2);

// From comp1 to comp2
Orientation direction(PositionComponent* comp1, PositionComponent* comp2);

}