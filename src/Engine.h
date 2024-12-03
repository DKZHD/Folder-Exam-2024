#pragma once
#include "Camera/Camera.h"
#include "Loaders/ShaderLoader.h"
#include "Scene/Scene.h"
#include "Window/Window.h"

class Engine
{
public:
	static Engine& GetInstance();
	void Init();
	void Run();
	Scene scene;
private:
	Window window;
	Camera camera;
	Shader shader;
	MathMesh m_mesh;

	float gravity = -9.81f;
};

