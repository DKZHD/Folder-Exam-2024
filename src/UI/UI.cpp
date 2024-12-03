#include "UI.h"
#include "../Engine.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../Mesh/Mesh.h"

void UI::init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Meshes", nullptr, ImGuiWindowFlags_NoCollapse);
	int index = 0;
	if(ImGui::Button("Add Ball",ImVec2(200,25)))
	{
		Engine::GetInstance().scene.AddMesh(MeshType::Sphere, glm::vec3(static_cast<float>(rand() % 800 - 400), 100.f , static_cast<float>(rand() % 800 - 400)), glm::vec3(2.f));
		std::string posString = "Mesh " + std::to_string(Engine::GetInstance().scene.GetMeshesSize()) + " Position";
		std::string velString = "Mesh " + std::to_string(Engine::GetInstance().scene.GetMeshesSize()) + " Velocity";
		AddDragFloat(posString, Engine::GetInstance().scene.GetMeshAt(Engine::GetInstance().scene.GetMeshesSize() - 1).position);
		AddDragFloat(velString, Engine::GetInstance().scene.GetMeshAt(Engine::GetInstance().scene.GetMeshesSize() - 1).velocity);
	}
	ImGui::SameLine();
	if(ImGui::Button("Add Multiple", ImVec2(200, 25)))
	{
		for(int i = 0; i<25;i++)
		{
			Engine::GetInstance().scene.AddMesh(MeshType::Sphere, glm::vec3(static_cast<float>(rand() % 800 - 400), 100.f, static_cast<float>(rand() % 800 - 400)), glm::vec3(2.f));
			std::string posString = "Mesh " + std::to_string(Engine::GetInstance().scene.GetMeshesSize()) + " Position";
			std::string velString = "Mesh " + std::to_string(Engine::GetInstance().scene.GetMeshesSize()) + " Velocity";
			AddDragFloat(posString, Engine::GetInstance().scene.GetMeshAt(Engine::GetInstance().scene.GetMeshesSize() - 1).position);
			AddDragFloat(velString, Engine::GetInstance().scene.GetMeshAt(Engine::GetInstance().scene.GetMeshesSize() - 1).velocity);
		}
	}
	for (auto& [name,vec3] : dragFloats)
	{
		ImGui::DragFloat3(name.c_str(), &vec3->x);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

void UI::AddDragFloat(std::string name, glm::vec3& vec)
{
	if (!dragFloats.contains(name))
		dragFloats[name] = &vec;
}
