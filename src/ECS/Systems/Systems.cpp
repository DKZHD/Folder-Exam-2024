#include "Systems.h"
#include <iostream>

#include "../../Engine.h"
#include "../ComponentManager/ComponentManager.h"
#include "../Components/Components.h"
#include "../EntityManager/EntityManager.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"

void HealthSystem::SetHealth(unsigned int id, int health)
{
	ComponentManager::GetHandler<HealthComponent>().GetComponent(id).health = health;
}

void HealthSystem::AddHealth(unsigned int id, int amount)
{
	ComponentManager::GetHandler<HealthComponent>().GetComponent(id).health += amount;
}

void HealthSystem::DecreaseHealth(unsigned int id, int amount)
{
	ComponentManager::GetHandler<HealthComponent>().GetComponent(id).health -= amount;
}

int HealthSystem::GetHealth(unsigned int id)
{
	return 	ComponentManager::GetHandler<HealthComponent>().GetComponent(id).health;
}

void CollisionSystem::updateBoundaries()
{
	std::vector<PositionComponent>& positions = ComponentManager::GetHandler<PositionComponent>().components;
	std::vector<CollisionComponent>& collisions = ComponentManager::GetHandler<CollisionComponent>().components;
	std::vector<MeshPropertyComponent>& meshProps = ComponentManager::GetHandler<MeshPropertyComponent>().components;

	for (size_t i = 0; i < Entity::globalID; ++i) 
	{
		glm::vec3 halfScale = meshProps[i].scale * 0.5f;
		collisions[i].boundingBox.min = positions[i].position - halfScale;
		collisions[i].boundingBox.max = positions[i].position + halfScale;
	}
}

glm::vec3 CollisionSystem::calculateNormal(glm::vec3& prevPos, const CollisionComponent& cc1, const CollisionComponent& cc2)
{
	glm::vec3 normal(0.0f);

	if (prevPos.x > cc1.boundingBox.max.x)
	{
		prevPos.x += cc1.boundingBox.max.x - cc2.boundingBox.min.x;
		normal = glm::vec3(1.f, 0.f, 0.f);
	}
	else if (prevPos.x < cc1.boundingBox.min.x)
	{
		prevPos.x -= cc2.boundingBox.max.x - cc1.boundingBox.min.x;
		normal = glm::vec3(-1.f, 0.f, 0.f);
	}
	if (prevPos.z > cc1.boundingBox.max.z)
	{
		prevPos.z += std::abs(cc1.boundingBox.max.z - cc2.boundingBox.min.z);
		normal = glm::vec3(0.f, 0.f, 1.f);
	}
	else if (prevPos.z < cc1.boundingBox.min.z)
	{
		prevPos.z -= std::abs(cc1.boundingBox.min.z - cc2.boundingBox.max.z);
		normal = glm::vec3(0.f, 0.f, -1.f);
	}
	if(prevPos.y > cc1.boundingBox.max.y)
		normal = glm::vec3(0.f,1.f,0.f);
	else if(prevPos.y < cc1.boundingBox.min.y)
		normal = glm::vec3(0.f,-1.f,0.f);
	return normal;
}

void CollisionSystem::checkAndResolveWorldCollision()
{
	std::vector<CollisionComponent>& collisionComps = ComponentManager::GetHandler<CollisionComponent>().components;
	std::vector<PositionComponent>& posComps = ComponentManager::GetHandler<PositionComponent>().components;
	std::vector<MovementComponent>& movementComps = ComponentManager::GetHandler<MovementComponent>().components;

	for(size_t id = 0; id < Entity::globalID; id++)
	{
		CollisionComponent& cc1 = collisionComps[id];
		for(size_t id2 = id+1; id2 < Entity::globalID; id2++)
		{
			CollisionComponent& cc2 = collisionComps[id2];
			if(cc1.type == MeshType::Sphere && cc2.type==MeshType::Sphere)
			{
				CheckBallBallCollision(id, id2);
			}

			if(checkCollision(cc1, cc2))
			{
				if(cc2.type == MeshType::Cube && cc1.type==MeshType::Sphere)
				{
					MovementComponent& movement = movementComps[id];
					glm::vec3& vel = movement.velocity;
					glm::vec3 prevPos = posComps[id].position;
				
					glm::vec3 normal = calculateNormal(prevPos, cc2, cc1);

					glm::vec3 newVel = glm::reflect(vel,normal);

					movement.velocity = newVel * 0.9f;
					posComps[id].position = prevPos;
				}
				else if(cc1.type == MeshType::Cube && cc2.type == MeshType::Sphere)
				{
					MovementComponent& movement = movementComps[id2];
					glm::vec3& vel = movement.velocity;
					glm::vec3 prevPos = posComps[id2].position;

					glm::vec3 normal = calculateNormal(prevPos, cc1, cc2);

					glm::vec3 newVal = glm::reflect(vel, normal);

					movement.velocity = newVal * 0.9f;
					posComps[id2].position = prevPos;
				}
			}
		}
	}
}

bool CollisionSystem::checkCollision(CollisionComponent& cc1, CollisionComponent& cc2)
{
	if(cc1.boundingBox.min.x < cc2.boundingBox.max.x && cc1.boundingBox.max.x > cc2.boundingBox.min.x && 
		cc1.boundingBox.min.y < cc2.boundingBox.max.y && cc1.boundingBox.max.y > cc2.boundingBox.min.y &&
		cc1.boundingBox.min.z < cc2.boundingBox.max.z && cc1.boundingBox.max.z > cc2.boundingBox.min.z)
	{
		return true;
	}
	return false;
}

bool CollisionSystem::CheckBallBallCollision(unsigned id, unsigned id2)
{
	PositionComponent& pc1 = ComponentManager::GetHandler<PositionComponent>().GetComponent(id);
	PositionComponent& pc2 = ComponentManager::GetHandler<PositionComponent>().GetComponent(id2);
	MovementComponent& mc1 = ComponentManager::GetHandler<MovementComponent>().GetComponent(id);
	MovementComponent& mc2 = ComponentManager::GetHandler<MovementComponent>().GetComponent(id2);
	MeshPropertyComponent& mpc1 = ComponentManager::GetHandler<MeshPropertyComponent>().GetComponent(id);
	MeshPropertyComponent& mpc2 = ComponentManager::GetHandler<MeshPropertyComponent>().GetComponent(id2);

	float dx = pc2.position.x - pc1.position.x;
	float dy = pc2.position.y - pc1.position.y;
	float dz = pc2.position.z - pc1.position.z;
	float l = sqrtf(dx * dx + dy * dy + dz * dz);
	float d = mpc1.scale.x + mpc2.scale.x;
	if (l >= d)
		return false;

	float detM = 2;
	float numM1 = 2 * 1;
	float numM2 = 2 * 1;

	float v1Num = glm::dot(mc1.velocity - mc2.velocity, pc1.position - pc2.position);
	float v2Num = glm::dot(mc2.velocity - mc1.velocity, pc2.position - pc1.position);

	float detV1 = glm::distance(pc1.position, pc2.position);
	float detV2 = glm::distance(pc2.position, pc1.position);

	glm::vec3 v1 = mc1.velocity - (numM2 / detM) * (v1Num / (detV1 * detV1)) * (pc1.position - pc2.position);
	glm::vec3 v2 = mc2.velocity - (numM1 / detM) * (v2Num / (detV2 * detV2)) * (pc2.position - pc1.position);

	mc1.velocity = v1;
	mc2.velocity = v2;

	glm::vec3 collisionV1 = glm::normalize(pc2.position - pc1.position);

	pc1.position = pc1.position + collisionV1 * ((l - d) / 2.f);
	pc2.position = pc2.position - collisionV1 * ((l - d) / 2.f);

	return true;
}

glm::vec3 MovementSystem::GetPosition(unsigned int id)
{
	return ComponentManager::GetHandler<PositionComponent>().GetComponent(id).position;
}

glm::vec3& MovementSystem::GetPositionREF(unsigned int id)
{
	return ComponentManager::GetHandler<PositionComponent>().GetComponent(id).position;
}

void MovementSystem::SetPosition(unsigned int id, glm::vec3 pos)
{
	ComponentManager::GetHandler<PositionComponent>().GetComponent(id).position = pos;
}

void MovementSystem::UpdatePosition(unsigned int id, glm::vec3 pos)
{
	ComponentManager::GetHandler<PositionComponent>().GetComponent(id).position += pos;
}

void MovementSystem::AddVelocity(unsigned int id, glm::vec3 vel)
{
	ComponentManager::GetHandler<MovementComponent>().GetComponent(id).velocity += vel;
}

void MovementSystem::DecreaseVelocity(unsigned int id, glm::vec3 vel)
{
	ComponentManager::GetHandler<MovementComponent>().GetComponent(id).velocity -= vel;
}

void MovementSystem::update(float deltaTime)
{
	std::vector<PositionComponent>& positions = ComponentManager::GetHandler<PositionComponent>().components;
	std::vector<MovementComponent>& velocities = ComponentManager::GetHandler<MovementComponent>().components;
	std::vector<MeshPropertyComponent>& meshProperties = ComponentManager::GetHandler<MeshPropertyComponent>().components;
	size_t entityCount = std::min(positions.size(), velocities.size());
	for (size_t i = 0; i < entityCount; i++) 
	{
		if(ComponentManager::GetHandler<CollisionComponent>().GetComponent(i).type == MeshType::Sphere)
		{
			positions[i].position += velocities[i].velocity * deltaTime;
			if (glm::length(glm::vec3(velocities[i].velocity.x, 0.f, velocities[i].velocity.z)) > 0.0f)
			{
				glm::vec3 rotationAxis = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), velocities[i].velocity));
				float rotationAmount = glm::length(glm::vec3(velocities[i].velocity.x, 0.f, velocities[i].velocity.z)) * deltaTime / meshProperties[i].scale.x;
				glm::mat4 incrementalRotation = glm::rotate(glm::mat4(1.0f), rotationAmount, rotationAxis);
				meshProperties[i].rotation = incrementalRotation * meshProperties[i].rotation;
			}
		}
	}
}

void MeshSystem::Render(unsigned int shaderProgram)
{
	auto& bufferComponents = ComponentManager::GetHandler<BufferComponent>().components;
	auto& collisionComps = ComponentManager::GetHandler<CollisionComponent>().components;

	for (size_t i = 0; i < bufferComponents.size(); ++i) 
	{
		if (!ComponentManager::GetHandler<BufferComponent>().HasComponent(i) || !ComponentManager::GetHandler<PositionComponent>().HasComponent(i))
			continue;
		BufferComponent& bc = ComponentManager::GetHandler<BufferComponent>().GetComponent(i);
		PositionComponent& pc = ComponentManager::GetHandler<PositionComponent>().GetComponent(i);

		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, pc.position);
		if (ComponentManager::GetHandler<MeshPropertyComponent>().HasComponent(i))
		{
			MeshPropertyComponent& mpc = ComponentManager::GetHandler<MeshPropertyComponent>().GetComponent(i);
			model *= mpc.rotation;
			model = glm::scale(model, mpc.scale);
		}

		glBindVertexArray(bc.vao);
		int uniformLoc = glGetUniformLocation(shaderProgram, "useCustomNormals");
		if(collisionComps[i].type==MeshType::Sphere)
		{
			glUniform1i(uniformLoc, true);
		}
		else
		{
			glUniform1i(uniformLoc, false);
		}
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "Model"), 1, GL_FALSE, &model[0][0]);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(bc.indices.size()), GL_UNSIGNED_INT, nullptr);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "Error: " << std::to_string(error) << '\n';
		}
	}
}

void MeshSystem::GenerateMesh(unsigned int id, MeshType type, glm::vec3 pos, glm::vec3 scale)
{
	ComponentManager::GetHandler<BufferComponent>().AddComponent(id);
	ComponentManager::GetHandler<PositionComponent>().AddComponent(id);
	ComponentManager::GetHandler<MeshPropertyComponent>().AddComponent(id);
	ComponentManager::GetHandler<CollisionComponent>().AddComponent(id);
	ComponentManager::GetHandler <MovementComponent>().AddComponent(id);

	ComponentManager::GetHandler<CollisionComponent>().GetComponent(id).type = type;

	BufferComponent& bc = ComponentManager::GetHandler<BufferComponent>().GetComponent(id);
	PositionComponent& pc = ComponentManager::GetHandler<PositionComponent>().GetComponent(id);
	MeshPropertyComponent& mpc = ComponentManager::GetHandler<MeshPropertyComponent>().GetComponent(id);

	pc.position = pos;
	mpc.scale = scale;

	if (type == MeshType::Cube)
	{

		bc.vertices = {
			glm::vec3(-0.5,-0.5,0.5),
			glm::vec3(0.5,-0.5,0.5),
			glm::vec3(0.5,0.5,0.5),
			glm::vec3(-0.5,0.5,0.5),
			glm::vec3(-0.5,-0.5,-0.5),
			glm::vec3(0.5,-0.5,-0.5),
			glm::vec3(0.5,0.5,-0.5),
			glm::vec3(-0.5,0.5,-0.5)
		};

		for (Vertex& vert : bc.vertices)
		{
			vert.color = glm::vec3(1.f);
		}

		bc.indices = {
			0,1,2,2,3,0,
			0,3,7,7,4,0,
			3,2,6,6,7,3,
			1,5,6,6,2,1,
			0,4,5,5,1,0,
			4,5,6,6,7,4
		};
	}
	if (type == MeshType::Sphere)
	{
		mpc.scale *= 0.5f;
		bc.vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
		bc.vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, -1.f));
		for (int i = 0;i < bc.vertices.size();i++)
		{
			glm::vec3 color(1.f, 0.f, 0.f);
			if (i >= bc.vertices.size()/2)
				color = glm::vec3(1.f);
			bc.vertices[i].color = color;
		}
		Subdivide(id, 0, 1, 2, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 2, 3, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 3, 4, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 4, 1, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 5, 2, 1, 3, glm::vec3(1.f, 1.f, 1.f));
		Subdivide(id, 5, 3, 2, 3, glm::vec3(1.f, 1.f, 1.f));
		Subdivide(id, 5, 4, 3, 3, glm::vec3(1.f, 1.f, 1.f));
		Subdivide(id, 5, 1, 4, 3, glm::vec3(1.f, 1.f, 1.f));
	}
	BindBuffers(id);
}

void MeshSystem::Subdivide(const unsigned& id, int index1, int index2, int index3, int i, glm::vec3 color)
{
	BufferComponent& bc = ComponentManager::GetHandler<BufferComponent>().GetComponent(id);
	if (i > 0) 
	{
		glm::vec3 pos1 = glm::normalize(bc.vertices[index1].pos + bc.vertices[index2].pos);
		int t_index1 = bc.vertices.size();
		bc.vertices.emplace_back(pos1, pos1);
		bc.vertices.back().color = color;

		glm::vec3 pos2 = glm::normalize(bc.vertices[index1].pos + bc.vertices[index3].pos);
		int t_index2 = bc.vertices.size();
		bc.vertices.emplace_back(pos2, pos2);
		bc.vertices.back().color = color;

		glm::vec3 pos3 = glm::normalize(bc.vertices[index3].pos + bc.vertices[index2].pos);
		int t_index3 = bc.vertices.size();
		bc.vertices.emplace_back(pos3, pos3);
		bc.vertices.back().color = color;

		Subdivide(id, index1, t_index1, t_index2, i - 1, color);
		Subdivide(id, index3, t_index2, t_index3, i - 1, color);
		Subdivide(id, index2, t_index3, t_index1, i - 1, color);
		Subdivide(id, t_index3, t_index2, t_index1, i - 1, color);
	}
	else 
	{
		bc.indices.emplace_back(index1);
		bc.indices.emplace_back(index2);
		bc.indices.emplace_back(index3);
	}
}

void MeshSystem::BindBuffers(unsigned int id)
{
	BufferComponent& bc = ComponentManager::GetHandler<BufferComponent>().GetComponent(id);
	glGenVertexArrays(1, &bc.vao);
	glGenBuffers(1, &bc.vbo);
	glGenBuffers(1, &bc.ebo);

	glBindVertexArray(bc.vao);
	glBindBuffer(GL_ARRAY_BUFFER, bc.vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * bc.vertices.size()), bc.vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bc.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(unsigned) * bc.indices.size()), bc.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glBindVertexArray(0);
}
