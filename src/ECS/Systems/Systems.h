#pragma once
#include "glm/vec3.hpp"
class CollisionComponent;
enum class MeshType;
class BufferComponent;


class ISystemBase
{
public:
	virtual ~ISystemBase() = default;
};

class HealthSystem : public ISystemBase
{
public:
	static void SetHealth(unsigned int id, int health);

	static void AddHealth(unsigned int id, int amount);

	static void DecreaseHealth(unsigned int id, int amount);

	static int GetHealth(unsigned int id);
};

class CollisionSystem : public ISystemBase
{
public:
	static void updateBoundaries();


	static void checkAndResolveWorldCollision();
private:
	static bool checkCollision(CollisionComponent& cc1, CollisionComponent& cc2);
	static glm::vec3 calculateNormal(glm::vec3& prevPos, const CollisionComponent& cc1, const CollisionComponent& cc2);

	static bool CheckBallBallCollision(unsigned id, unsigned id2);
};

class MovementSystem : public ISystemBase
{
public:
	static glm::vec3 GetPosition(unsigned int id);

	static glm::vec3& GetPositionREF(unsigned int id);

	static void SetPosition(unsigned int id, glm::vec3 pos);

	static void UpdatePosition(unsigned int id, glm::vec3 pos);

	static void AddVelocity(unsigned int id, glm::vec3 vel);

	static void DecreaseVelocity(unsigned int id, glm::vec3 vel);

	static void update(float deltaTime);
};

class MeshSystem
{
public:
	static void Render(unsigned int shaderProgram);

	// Generates mesh for specified id, scale works as radius in the case of a sphere
	static void GenerateMesh(unsigned int id, MeshType type, glm::vec3 pos, glm::vec3 scale);


private:
	static void BindBuffers(unsigned int id);

	static void Subdivide(const unsigned& id, int index1, int index2, int index3, int i, glm::vec3 color);

};