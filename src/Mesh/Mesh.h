#pragma once
#include <vector>
#include "glm/matrix.hpp"
#include "glm/vec3.hpp"


struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal = glm::vec3(0.f);
	glm::vec3 color = glm::vec3(0.f);
	Vertex() = default;
	Vertex(glm::vec3 pos) : pos(pos){}
	Vertex(glm::vec3 pos, glm::vec3 normal) : pos(pos), normal(normal){}
};

struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};

enum class MeshType
{
	Sphere,
	Cube
};

class Mesh
{
public:
	Mesh() = default;
	Mesh(MeshType type, glm::vec3 pos, glm::vec3 scale);
	void Subdivide(int index1, int index2, int index3, int i, glm::vec3 color);

	void UpdatePosition(glm::vec3 pos);
	void SetPosition(glm::vec3 pos);
	glm::vec3& GetPosition() const;

	void UpdateVelocity(glm::vec3 vel);
	bool checkCollision(Mesh& mesh);
	void UpdateBoundingBox();

	void Update(float deltaTime, float gravity);
	void Render(uint32_t program) const;
	void TrackBall(float deltaTime);

	glm::vec3 velocity = glm::vec3(0.f);

	unsigned int id = 0;

	AABB boundingBox;

private:
	void BindBuffer();

	unsigned tracking_vbo = 0, tracking_vao = 0;
	std::vector<Vertex> trackingPoints;
	std::vector<Vertex> controlPoints;

	float mass = 2.f;
	float radius = 10.f;
	float timer = 0.f;

	glm::mat4 rotationMatrix = glm::mat4(1.f);
	glm::vec3 scale;
};

class MathMesh
{
public:
	MathMesh() = default;
	MathMesh(std::vector<glm::vec3> v, int prRow, bool point_cloud = false);
	void SetPosition(glm::vec3 v) { position = v; }
	void Render(uint32_t program, bool point_cloud = false) const;
	float GetHeightAtPosition(glm::vec3 pos);
	glm::vec3 GetNormalAtPosition(glm::vec3 pos);
	float GetFrictionCoEfficient(glm::vec3 pos);
private:
	void BindBuffer();
	glm::vec3 BarycentricCalculationVec(unsigned index0, unsigned index1, unsigned index2, glm::vec3 pos);
	float BarycentricCalculation(unsigned index0,unsigned index1,unsigned index2, glm::vec3 pos);
	std::tuple<unsigned,unsigned,unsigned> GetTriangleIndicesAtPositon(glm::vec3 pos);

	uint32_t vbo = 0, ebo = 0, vao = 0;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> normals;

	int numPrRow = 0;

	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(0.f);
};


