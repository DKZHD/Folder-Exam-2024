#include "Systems.h"

#include <iostream>

#include "../ComponentManager/ComponentManager.h"
#include "../Components/Components.h"
#include "../EntityManager/EntityManager.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"

void HealthSystem::SetHealth(unsigned int id, int health)
{
	ComponentManager::GetInstance().GetComponent<HealthComponent>(id).health = health;
}

void HealthSystem::AddHealth(unsigned int id, int amount)
{
	ComponentManager::GetInstance().GetComponent<HealthComponent>(id).health += amount;
}

void HealthSystem::DecreaseHealth(unsigned int id, int amount)
{
	ComponentManager::GetInstance().GetComponent<HealthComponent>(id).health -= amount;
}

int HealthSystem::GetHealth(unsigned int id)
{
	return ComponentManager::GetInstance().GetComponent<HealthComponent>(id).health;
}

void CollisionSystem::updateBoundaries(unsigned int id)
{
	if(!ComponentManager::GetInstance().HasComponents<CollisionComponent,PositionComponent,MeshPropertyComponent>(id))
	{
		return;
	}
	CollisionComponent& cc = ComponentManager::GetInstance().GetComponent<CollisionComponent>(id);
	PositionComponent& pc = ComponentManager::GetInstance().GetComponent<PositionComponent>(id);
	MeshPropertyComponent& mpc = ComponentManager::GetInstance().GetComponent<MeshPropertyComponent>(id);

	glm::vec3 halfScale = mpc.scale * 0.5f;

	cc.boundingBox.min = pc.position - halfScale;
	cc.boundingBox.max = pc.position + halfScale;

}

glm::vec3 MovementSystem::GetPosition(unsigned int id)
{
	return ComponentManager::GetInstance().GetComponent<PositionComponent>(id).position;
}

glm::vec3& MovementSystem::GetPositionREF(unsigned int id)
{
	return ComponentManager::GetInstance().GetComponent<PositionComponent>(id).position;
}

void MovementSystem::SetPosition(unsigned int id, glm::vec3 pos)
{
	ComponentManager::GetInstance().GetComponent<PositionComponent>(id).position = pos;
}

void MovementSystem::UpdatePosition(unsigned int id, glm::vec3 pos)
{
	ComponentManager::GetInstance().GetComponent<PositionComponent>(id).position += pos;
}

void MovementSystem::AddVelocity(unsigned int id, glm::vec3 vel)
{
	ComponentManager::GetInstance().GetComponent<MovementComponent>(id).velocity += vel;
}

void MovementSystem::DecreaseVelocity(unsigned int id, glm::vec3 vel)
{
	ComponentManager::GetInstance().GetComponent<MovementComponent>(id).velocity -= vel;
}

void MovementSystem::update(float deltaTime)
{
	for(unsigned id = 0;id<Entity::globalID;id++)
	{
		if(ComponentManager::GetInstance().HasComponents<MovementComponent,PositionComponent>(id))
		{
			GetPositionREF(id) += ComponentManager::GetInstance().GetComponent<MovementComponent>(id).velocity * deltaTime;
		}
	}
}

void MeshSystem::Render(unsigned int shaderProgram)
{
	for (unsigned id = 0; id < Entity::globalID; id++)
	{
		if (!ComponentManager::GetInstance().HasComponents<BufferComponent, PositionComponent>(id))
			continue;
		BufferComponent& bc = ComponentManager::GetInstance().GetComponent<BufferComponent>(id);
		PositionComponent& pc = ComponentManager::GetInstance().GetComponent<PositionComponent>(id);

		glBindVertexArray(bc.vao);
		glm::mat4 model(1.f);
		model = glm::translate(model, pc.position);

		if(ComponentManager::GetInstance().HasComponents<MeshPropertyComponent>(id))
		{
			model = glm::scale(model, ComponentManager::GetInstance().GetComponent<MeshPropertyComponent>(id).scale);
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
	if(!ComponentManager::GetInstance().HasComponents<BufferComponent,PositionComponent,MeshPropertyComponent, CollisionComponent>(id))
	{
		ComponentManager::GetInstance().addComponents<BufferComponent, PositionComponent, MeshPropertyComponent, CollisionComponent>(id);
	}
	BufferComponent& bc = ComponentManager::GetInstance().GetComponent<BufferComponent>(id);
	PositionComponent& pc = ComponentManager::GetInstance().GetComponent<PositionComponent>(id);
	MeshPropertyComponent& mpc = ComponentManager::GetInstance().GetComponent<MeshPropertyComponent>(id);
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
		bc.vertices.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
		bc.vertices.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
		bc.vertices.emplace_back(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 0.f, -1.f));
		for (Vertex& vert : bc.vertices)
		{
			vert.color = glm::vec3(1.f, 0.f, 0.f);
		}
		Subdivide(id, 0, 1, 2, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 2, 3, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 3, 4, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 0, 4, 1, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 5, 2, 1, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 5, 3, 2, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 5, 4, 3, 3, glm::vec3(1.f, 0.f, 0.f));
		Subdivide(id, 5, 1, 4, 3, glm::vec3(1.f, 0.f, 0.f));
	}
	BindBuffers(id);
}

void MeshSystem::Subdivide(const unsigned& id, int index1, int index2, int index3, int i, glm::vec3 color)
{
	BufferComponent& bc = ComponentManager::GetInstance().GetComponent<BufferComponent>(id);
	if (i > 0) {
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
	else {
		bc.indices.emplace_back(index1);
		bc.indices.emplace_back(index2);
		bc.indices.emplace_back(index3);
	}
}

void MeshSystem::BindBuffers(unsigned int id)
{
	BufferComponent& bc = ComponentManager::GetInstance().GetComponent<BufferComponent>(id);
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
