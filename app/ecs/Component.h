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

// Helper macro
#define COMPONENT_CLONE_AND_ID(name, compId) \
ecs::Component* clone() override { \
  auto comp = new name(); \
  *comp = *this; \
  return comp; \
} \
ecs::ComponentID id() override { \
  return compId; \
}