#include "Engine.h"

#include <chrono>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Input/Input.h"
#include "Loaders/fileLoader.h"
#include "Math/Math.h"
#include "UI/UI.h"

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


	switch (3)
	{
	case 1:
		fileLoader::ReadBinaryFile("PointClouds/PointsBin.dat");
		Input::Keyboard::pointCloud = true;
		scene.GenerateTerrain(fileLoader::filePosition, 1);
		break;
	case 3:
		fileLoader::ReadBinaryFile("PointClouds/BSplineSurfaceBin2.dat");
		scene.GenerateTerrain(fileLoader::filePosition, 999);
		scene.AddMesh(MeshType::Sphere, glm::vec3(0.f, 100.f, 0.f), glm::vec3(2.f));
		scene.AddMesh(MeshType::Sphere, glm::vec3(20.f, 100.f, 0.f), glm::vec3(2.f));
		break;
	case 2:
		fileLoader::ReadBinaryFile("PointClouds/PointsSortedBin.dat");
		scene.GenerateTerrain(fileLoader::filePosition, 1597);
		break;
	}
	
	std::vector<std::vector<glm::vec3>> controlPoints = {
			{ glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(2, 0, 0), glm::vec3(3, 0, 0) },
			{ glm::vec3(0, 1, 0), glm::vec3(1, 1, 2), glm::vec3(2, 1, 2), glm::vec3(3, 1, 0) },
			{ glm::vec3(0, 2, 0), glm::vec3(1, 2, 0), glm::vec3(2, 2, 0), glm::vec3(3, 2, 0) }
	};
	m_mesh = MathMesh(Math::CalculateSurface(50, controlPoints),50);

	//fileLoader::ReadBinaryFile("PointClouds/PointsBin.dat");
    //fileLoader::LoadFile("BSplineSurface.txt");
    //auto pair = Math::CalculateChunks(fileLoader::filePosition, 4.f);
    //std::vector<glm::vec3> terrain = Math::GenerateTerrainBSplineSurface(fileLoader::filePosition, 2.f, 10, 2, 2);
    //std::vector<glm::vec3> terrain = Math::CalculateSurface(1000, Math::Create2DVectorFrom1D(fileLoader::filePosition));
    //float size = 1000.f;
    //std::vector<glm::vec3> v = { glm::vec3(-size,500.f,-size),glm::vec3(size,500.f,-size),glm::vec3(-size,500.f,size),glm::vec3(size,0.f,size) };
    //scene.GenerateTerrain(terrain, 1000);
    //scene.GenerateTerrain(v,2);
}

void Engine::Run()
{
	float deltaTime = 0.f;
	float lastFrame = static_cast<float>(glfwGetTime());
	bool collisionEnabled = true;
	UI ui;
	ui.init(window);
	ui.AddDragFloat("Mesh 1 Position", scene.GetMeshAt(0).position);
	ui.AddDragFloat("Mesh 1 Velocity", scene.GetMeshAt(0).velocity);
	ui.AddDragFloat("Mesh 2 Position", scene.GetMeshAt(1).position);
	ui.AddDragFloat("Mesh 2 Velocity", scene.GetMeshAt(1).velocity);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
		scene.UpdateScene(deltaTime,gravity, collisionEnabled);
		m_mesh.Render(shader.GetProgram());

		ui.render();
		Input::processInput(window, camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
