#include "Scene.h"

#include "../ECS/Systems/Systems.h"
#include "glm/vec3.hpp"

void Scene::RenderScene(uint32_t program)
{
	//terrain.Render(program);
	MeshSystem::Render(program);
}
