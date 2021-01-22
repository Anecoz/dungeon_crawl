#include "Engine.h"

#include <algorithm>

namespace ecs {

void Engine::addEntity(Entity&& entity)
{
  _entities.emplace_back(std::move(entity));
}

std::vector<Entity*> Engine::getEntitiesWithComp(ComponentID comp)
{
  std::vector<Entity*> output;
  for (auto& entity: _entities) {
    if (entity.hasComp(comp)) {
      output.emplace_back(&entity);
    }
  }
  return output;
}

void Engine::addSystem(std::unique_ptr<System> system)
{
  _systems.emplace_back(std::move(system));
  std::sort(_systems.begin(), _systems.end(),
  [] (const std::unique_ptr<System>& systemA, const std::unique_ptr<System>& systemB) {
    return systemA->priority() < systemB->priority();
  });
}

void Engine::runOnce()
{
  for (auto& system: _systems) {
    system->run(*this);
  }
}

}