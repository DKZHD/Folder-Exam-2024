#pragma once
#include <vector>
#include "glm/vec3.hpp"
#include "../../Mesh/Mesh.h"

class HealthComponent
{
public:
	int health = 100;
};
class PositionComponent
{
public:
	glm::vec3 position = glm::vec3(0.f);
};
class MovementComponent
{
public:
	glm::vec3 velocity = glm::vec3(0.f);
};
class BufferComponent
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	unsigned int vao = 0, vbo = 0, ebo = 0;
};

class MeshPropertyComponent
{
public:
	glm::vec3 scale = glm::vec3(0.f);
};

class CollisionComponent
{
public:
	AABB boundingBox;
};