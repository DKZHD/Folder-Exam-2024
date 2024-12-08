#pragma once
#include "../Mesh/Mesh.h"

class Scene
{
public:
	unsigned AddMesh(MeshType type, glm::vec3 pos, glm::vec3 scale);
	void RenderScene(uint32_t program);
	void UpdateScene(float deltaTime, float gravity, bool collision);
	void GenerateTerrain(const std::vector<glm::vec3>& cloud, int numPrRow);
	MathMesh& GetTerrain() const;
	Mesh& GetMeshAt(uint32_t index);
	uint32_t GetMeshesSize();
	std::vector<Mesh>& GetMeshes();

private:
	std::vector<Mesh> meshes;
	MathMesh terrain;
};

