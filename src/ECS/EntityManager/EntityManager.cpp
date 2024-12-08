#include "EntityManager.h"

unsigned EntityManager::AddEntity()
{
	entities.emplace_back();
	return Entity::globalID-1;
}

unsigned EntityManager::GetLastEntity()
{
	return entities.back().ID;
}

EntityManager& EntityManager::GetInstance()
{
	static EntityManager entity_manager;
	return entity_manager;
}

Entity& EntityManager::GetEntity(size_t index)
{
	if (index > entities.size() - 1)
		throw;
	return entities[index];
}
