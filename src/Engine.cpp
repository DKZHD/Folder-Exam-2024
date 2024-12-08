#include "Engine.h"

#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "Input/Input.h"
#include "ParticleSystem/ParticleSystem.h"

Engine& Engine::GetInstance()
{
	static Engine engine;
	return engine;
}

void Engine::Init()
{
	window.init();
	Input::Mouse::cameraPtr = &camera;
	shader.loadShader("ShaderSrc/VertexShader.glsl", "ShaderSrc/FragmentShader.glsl");

	luaCustom.init();
	luaCustom.LoadInitialFIles();
}

void Engine::Run()
{
	float lastFrame = static_cast<float>(glfwGetTime());
	bool collisionEnabled = true;
	Shader particleShader;
	particleShader.loadShader("ShaderSrc/ParticleVertShader.glsl", "ShaderSrc/ParticleFragShader.glsl");
	ParticleMesh mesh(particle_system);

	ui.init(window);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Window::ClearBuffers();
		shader.Use();
		camera.UpdateMats(shader.GetProgram());
		shader.SetVec3("viewPos", camera.getCamPos());
		scene.RenderScene(shader.GetProgram());

		particleShader.Use();
		camera.UpdateMats(particleShader.GetProgram());
		particle_system.EmitParticles(deltaTime);
		particle_system.UpdatePositions(deltaTime);
		mesh.UpdateInstancedBuffer(particle_system);
		glm::mat4 model(1.f);
		model = glm::scale(model, glm::vec3(0.01f));
		glUniformMatrix4fv(glGetUniformLocation(particleShader.GetProgram(), "model"), 1, GL_FALSE, &model[0][0]);
		mesh.Render(particleShader.GetProgram());

		//scene.UpdateScene(deltaTime,gravity, collisionEnabled);

		ui.render();
		Input::processInput(window, camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

float Engine::GetDeltaTime()
{
	return deltaTime;
}

GLFWwindow* Engine::GetWindow()
{
	return window;
}
