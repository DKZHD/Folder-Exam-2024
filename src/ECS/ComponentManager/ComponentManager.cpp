#include "ComponentManager.h"
#include "../Components/Components.h"


ComponentManager& ComponentManager::GetInstance()
{
	static ComponentManager component_manager;
	return component_manager;
}

ComponentManager::ComponentManager()
{
	registerType<HealthComponent>();
	registerType<PositionComponent>();
	registerType<MovementComponent>();
	registerType<BufferComponent>();
	registerType<MeshPropertyComponent>();
	registerType<CollisionComponent>();
}



