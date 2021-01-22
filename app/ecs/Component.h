#pragma once

#include <cstdint>

namespace ecs {

typedef std::uint64_t ComponentID;

class Component
{
public:
  Component() {}
  virtual ~Component() {}
  virtual Component* clone() = 0;
  virtual ComponentID id() = 0;
};

}