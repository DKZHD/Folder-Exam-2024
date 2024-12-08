#pragma once
#include "Camera/Camera.h"
#include "Loaders/ShaderLoader.h"
#include "Lua/LuaCustom.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Scene/Scene.h"
#include "sol/state.hpp"
#include "UI/UI.h"
#include "Window/Window.h"

class Engine
{
public:
	static Engine& GetInstance();
	void Init();
	void Run();
	float GetDeltaTime();
	GLFWwindow* GetWindow();
	Scene scene;
	LuaCustom luaCustom;
	UI ui;
	Camera camera;
	ParticleSystem particle_system;

private:
	Window window;
	Shader shader;
	float deltaTime = 0.f;
	float gravity = -9.81f;
};

