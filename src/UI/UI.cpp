#include "UI.h"
#include "../Engine.h"
#include "../../Raycast.h"
#include "../ECS/ComponentManager/ComponentManager.h"
#include "../ECS/Components/Components.h"
#include "../ECS/EntityManager/EntityManager.h"
#include "../ECS/Systems/Systems.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../Mesh/Mesh.h"

void UI::init(GLFWwindow* window)
{
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void UI::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::Begin("##FPS", nullptr,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	ImGui::Text("FPS: %f", 1.f/Engine::GetInstance().GetDeltaTime());
	ImGui::Text("Amount of Particles: %d", Engine::GetInstance().particle_system.activeParticles);
	ImGui::End();
	ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoCollapse);
	int index = 0;
	if(ImGui::Button("Add Ball",ImVec2(200,25)))
	{
		unsigned id = EntityManager::GetInstance().AddEntity();
		MeshSystem::GenerateMesh(id, MeshType::Sphere, glm::vec3(static_cast<float>(rand() % 800 - 400), 100.f, static_cast<float>(rand() % 800 - 400)), glm::vec3(2.f));
		std::string posString = "Mesh " + std::to_string(id) + " Position";
		AddDragFloat(posString, id);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add Multiple", ImVec2(200, 25)))
	{
		for(int i = 0; i<25;i++)
		{
			unsigned id = EntityManager::GetInstance().AddEntity();
			MeshSystem::GenerateMesh(id, MeshType::Sphere, glm::vec3(static_cast<float>(rand() % 800 - 400), 100.f, static_cast<float>(rand() % 800 - 400)), glm::vec3(2.f));
			std::string posString = "Mesh " + std::to_string(id) + " Position";
			AddDragFloat(posString, id);
		}
	}
	for (dragFloatInfo& info : dragFloats)
	{
		glm::vec3& vec3 = ComponentManager::GetInstance().GetComponent<PositionComponent>(info.id).position;
		ImGui::DragFloat3(info.displayText.c_str(), &vec3.x,0.2f);
	}
	ImGui::End();
	if(Raycast::GetActiveID()!=-1)
	{
		glm::vec3& vec3 = ComponentManager::GetInstance().GetComponent<PositionComponent>(Raycast::GetActiveID()).position;
		ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoCollapse);
		ImGui::DragFloat3("Active Mesh Position", &vec3.x, 0.2f);
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

void UI::AddDragFloat(std::string name, unsigned int id)
{
	if (stringSet.contains(name))
	{
		return;
	}
	stringSet.insert(name);
	dragFloats.emplace_back(name,id);
}

