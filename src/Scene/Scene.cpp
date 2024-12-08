#include "Scene.h"

#include "../ECS/Systems/Systems.h"
#include "glm/vec3.hpp"

unsigned Scene::AddMesh(MeshType type, glm::vec3 pos, glm::vec3 scale)
{
	meshes.emplace_back(type,pos,scale);
	return meshes.back().id;
}

void Scene::UpdateScene(float deltaTime, float gravity, bool collision)
{
	for(Mesh& mesh : meshes)
	{
		if(collision)
		{
			for(Mesh& mesh2 : meshes)
			{
				if(&mesh == &mesh2)
					continue;
				mesh.checkCollision(mesh2);
			}
		}
		mesh.Update(deltaTime,gravity);
		mesh.TrackBall(deltaTime);
	}
}

void Scene::GenerateTerrain(const std::vector<glm::vec3>& cloud, int numPrRow)
{
	terrain = MathMesh(cloud, numPrRow, false);
}

MathMesh& Scene::GetTerrain() const
{
	return const_cast<MathMesh&>(terrain);
}

Mesh& Scene::GetMeshAt(uint32_t index)
{
	return meshes[index];
}

uint32_t Scene::GetMeshesSize()
{
	return meshes.size();
}

std::vector<Mesh>& Scene::GetMeshes()
{
	return meshes;
}

void Scene::RenderScene(uint32_t program)
{
	//terrain.Render(program);
	MeshSystem::Render(program);
}
