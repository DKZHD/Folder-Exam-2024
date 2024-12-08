#include "ParticleSystem.h"

#include <random>
#include <thread>

#include "glad/glad.h"
#include "glm/gtc/constants.hpp"

ParticleMesh::ParticleMesh(ParticleSystem& particle_system)
{
	vertices = {
		glm::vec3(-0.5,-0.5,0.5),
		glm::vec3(0.5,-0.5,0.5),
		glm::vec3(0.5,0.5,0.5),
		glm::vec3(-0.5,0.5,0.5),
		glm::vec3(-0.5,-0.5,-0.5),
		glm::vec3(0.5,-0.5,-0.5),
		glm::vec3(0.5,0.5,-0.5),
		glm::vec3(-0.5,0.5,-0.5)
	};

	indices = {
		0,1,2,2,3,0,
		0,3,7,7,4,0,
		3,2,6,6,7,3,
		1,5,6,6,2,1,
		0,4,5,5,1,0,
		4,5,6,6,7,4
	};
	BindBuffer(particle_system);
	
}

void ParticleMesh::BindBuffer(ParticleSystem& particle_system)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &instanceVbo);
	glGenBuffers(1, &lifetimeVbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(unsigned) * indices.size()), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(glm::vec3) * ParticleSystem::MAX_PARTICLES), particle_system.Positions, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, lifetimeVbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * ParticleSystem::MAX_PARTICLES), particle_system.Lifetimes, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
}

void ParticleMesh::UpdateInstancedBuffer(ParticleSystem& particle_system)
{
	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER,0,static_cast<GLsizeiptr>(sizeof(glm::vec3)*ParticleSystem::MAX_PARTICLES),particle_system.Positions);
	glBindBuffer(GL_ARRAY_BUFFER, lifetimeVbo);
	glBufferSubData(GL_ARRAY_BUFFER,0,static_cast<GLsizeiptr>(sizeof(float)*ParticleSystem::MAX_PARTICLES),particle_system.Lifetimes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleMesh::Render(unsigned shader)
{
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(ParticleSystem::MAX_PARTICLES));
	glBindVertexArray(0);
}

void ParticleSystem::UpdatePositions(float deltaTime)
{
	for(size_t i = 0; i<MAX_PARTICLES;i++)
	{
		if(Lifetimes[i]<=0.f)
			continue;
		Velocities[i] = glm::clamp(Velocities[i]+=glm::vec3(0.f,-9.81f,0.f)*deltaTime, glm::vec3(0.f, -9.81f, 0.f)-wind, glm::vec3(0.f, 9.81f, 0.f)+wind);
		Positions[i] += Velocities[i] * deltaTime;
		Lifetimes[i] -= deltaTime;

		if (Positions[i].y-0.005f <= 1.f)
		{
			Positions[i].y = 1.005f;
			Velocities[i] *= -0.3f;
			Velocities[i].x = 0.f;
			Lifetimes[i] = 0.5f;
		}
		if (Lifetimes[i] <= 0)
			activeParticles--;
	}
}

void ParticleSystem::EmitParticles(float deltaTime)
{
	timer += deltaTime;
	if (timer > spawnRate)
	{
		std::default_random_engine engine(std::random_device{}());
		std::uniform_real_distribution<float> random(-25.f, 25.f);
		int spawned = 0;
		timer = 0.f;
		for(size_t i = 0; i< MAX_PARTICLES;i++)
		{
			if(Lifetimes[i]<=0.f)
			{
				Positions[i] = glm::vec3(random(engine), systemPos.y, random(engine));
				Velocities[i] = glm::vec3(0.f, -9.81f, 0.f) + wind;
				Lifetimes[i] = defaultLifetime;
				activeParticles++;

				spawned++;
				if(spawned>=100)
				{
					break;
				}
			}
		}
	}
}