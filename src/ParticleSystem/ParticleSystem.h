#pragma once
#include "glm/vec3.hpp"
#include "../Mesh/Mesh.h"
#include <array>

class ParticleSystem;

class ParticleMesh
{
public:
	ParticleMesh(ParticleSystem& particle_system);
	void BindBuffer(ParticleSystem& particle_system);
	void UpdateInstancedBuffer(ParticleSystem& particle_system);
	void Render(unsigned shader);
private:
	std::array<Vertex,8> vertices;
	std::array<unsigned,36> indices;
	unsigned vao = 0, vbo = 0, ebo = 0, instanceVbo = 0, lifetimeVbo = 0;
};

class ParticleSystem
{
public:
	glm::vec3 systemPos = glm::vec3(0.f,35.f,0.f);
	glm::vec3 wind = glm::vec3(2.f,0.f,0.f);

	static constexpr size_t MAX_PARTICLES = 100000;

	glm::vec3 Positions[MAX_PARTICLES];
	glm::vec3 Velocities[MAX_PARTICLES];
	float Lifetimes[MAX_PARTICLES];

	void UpdatePositions(float deltaTime);
	void EmitParticles(float deltaTime);
	float timer = 0.f;
	float spawnRate = 0.001f;
	float defaultLifetime = 5.f;
	size_t activeParticles = 0;
};

