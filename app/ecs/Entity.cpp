#include "Entity.h"

#include <string>
#include <iostream>

namespace ecs {

namespace {

static EntityID g_Id = 0;

}

Entity::Entity()
  : _id(g_Id++)
{}

Entity::~Entity()
{}

/*Entity::Entity(const Entity& other)
  : _id(g_Id++)
{
  _components.reserve(other._components.size());
  for (auto& comp : other._components) {
    _components.emplace_back(std::unique_ptr<Component>(comp->clone()));
  }
}*/

Entity::Entity(Entity&& other)
  : _components(std::move(other._components))
  , _id(other._id)
{
  other._components.clear();
}

/*Entity& Entity::operator=(const Entity& other)
{
  if (this != &other) {
    _components.reserve(other._components.size());
    for (auto& comp : other._components) {
      _components.emplace_back(std::unique_ptr<Component>(comp->clone()));
    }
    _id = g_Id++;
  }
  return *this;
}*/

Entity& Entity::operator=(Entity&& other)
{
  if (this != &other) {
    _components = std::move(other._components);
    _id = other._id;
    other._components.clear();
  }
  return *this;
}

bool Entity::hasComp(ComponentID compId) const
{
  for (auto& comp: _components) {
    if (comp->id() == compId) return true;
  }
  return false;
}

Component* Entity::getComp(ComponentID compId)
{
  for (auto& comp: _components) {
    if (comp->id() == compId) {
      return comp.get();
    }
  }
  return nullptr;
}

std::unique_ptr<Component> Entity::takeComp(ComponentID compId)
{
  for (auto it = _components.begin(); it != _components.end();) {
    if ((*it)->id() == compId) {
      auto comp = std::move(_components[it - _components.begin()]);
      it = _components.erase(it);
      return std::move(comp);
    }
    ++it;
  }
  return nullptr;
}

void Entity::addComp(std::unique_ptr<Component> comp)
{
  if (hasComp(comp->id())) {
    std::cerr << "Warning! Tried to add a component to entity " << std::to_string(_id) << " but it already exists." << std::endl;
    return;
  }

  std::cout << "Adding component with id " << std::to_string(comp->id()) << " to entity with id " << std::to_string(_id) << std::endl;
  _components.emplace_back(std::move(comp));
}

void Entity::removeComp(ComponentID compId)
{
  for (auto it = _components.begin(); it != _components.end(); ++it) {
    if ((*it)->id() == compId) {
      _components.erase(it);
      return;
    }
  }
}

}