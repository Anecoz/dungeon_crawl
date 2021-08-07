#pragma once

#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <memory>
#include <vector>

namespace ecs {

class Engine
{
public:
  Engine() = default;
  ~Engine() = default;

  Engine(const Engine&) = delete;
  Engine(Engine&&) = delete;
  Engine& operator=(const Engine&) = delete;
  Engine& operator=(Engine&&) = delete;

  void addEntity(std::unique_ptr<Entity>&& entity);
  void removeEntity(EntityID id);
  std::vector<Entity*> getEntitiesWithComp(ComponentID comp);
  Entity* getEntityById(EntityID id);

  void addSystem(std::unique_ptr<System> system);

  void runOnce();
private:
  std::vector<std::unique_ptr<Entity>> _entities;
  std::vector<std::unique_ptr<System>> _systems;
};

}