#pragma once

#include "Component.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace ecs {

typedef std::uint64_t EntityID;

class Entity
{
public:
  Entity();
  ~Entity();

  Entity(const Entity& other);
  Entity(Entity&& other);

  Entity& operator=(const Entity& other);
  Entity& operator=(Entity&& other);

  EntityID id() { return _id; }

  bool hasComp(ComponentID compId) const;
  Component* getComp(ComponentID compId);
  void addComp(std::unique_ptr<Component> comp);
  void removeComp(ComponentID compId);

private:
  std::vector<std::unique_ptr<Component>> _components;
  EntityID _id;
};

}