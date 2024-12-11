#include "Input.h"
#include <algorithm>

#include "../Engine.h"
#include "../../Raycast.h"
#include "../Camera/Camera.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/detail/func_trigonometric.inl"
#include "glm/ext/quaternion_geometric.hpp"
#include "imgui/imgui.h"

void Input::processInput(GLFWwindow* window, Camera& camera)
{
	if(glfwGetKey(window,GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window,true);
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.UpdateCamPos(Mouse::cameraPtr->camSpeed *glm::normalize(glm::cross(camera.getCamFwd(),glm::vec3(0.f,1.f,0.f))));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.UpdateCamPos(-Mouse::cameraPtr->camSpeed * glm::normalize(glm::cross(camera.getCamFwd(), glm::vec3(0.f, 1.f, 0.f))));
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.UpdateCamPos(Mouse::cameraPtr->camSpeed * camera.getCamFwd());
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.UpdateCamPos(-Mouse::cameraPtr->camSpeed * camera.getCamFwd());
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		camera.UpdateCamPos(Mouse::cameraPtr->camSpeed * glm::vec3(0.f,1.f,0.f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		camera.UpdateCamPos(-Mouse::cameraPtr->camSpeed * glm::vec3(0.f, 1.f, 0.f));
	}
}

void Input::Keyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action==GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_F:
			if(glfwGetInputMode(window,GLFW_CURSOR)==GLFW_CURSOR_DISABLED)
			{
				Input::Mouse::firstMouse = true;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}


			break;

		case GLFW_KEY_R:
			try
			{
				Engine::GetInstance().luaCustom.LoadFiles("RunningScript.lua");
			}
			catch (const sol::error& e)
			{
				std::cout << "Error with Lua script: " << e.what() << '\n';
			}
			break;
		case GLFW_KEY_TAB:
			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframe = false;
			}
				
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframe = true;
			}
			break;
		case GLFW_KEY_P:
			if(pointCloud)
			{
				pointCloud = false;
			}
			else
			{
				pointCloud = true;
			}
			break;
		default:
			break;
	}
	}

}

void Input::Mouse::MouseMoveCallback(GLFWwindow* window, double xPos, double yPos)
{
	if(glfwGetInputMode(window,GLFW_CURSOR)!=GLFW_CURSOR_DISABLED)
		return;
	if(firstMouse)
	{
		prevX = xPos;
		prevY = yPos;
		firstMouse = false;
	}
	double xOffset = xPos - prevX;
	double yOffset = prevY - yPos;
	prevX = xPos;
	prevY = yPos;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += static_cast<float>(xOffset);
	pitch += static_cast<float>(yOffset);

	if (pitch > 89.f)
		pitch = 89.f;
	else if (pitch < -89.f)
		pitch = -89.f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	if (cameraPtr)
		cameraPtr->getCamFwd() = glm::normalize(direction);
}

void Input::Mouse::MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	cameraPtr->camSpeed = std::clamp(cameraPtr->camSpeed + static_cast<float>(y_offset*0.05f),0.05f,20.f);
}

void Input::Mouse::MouseClickCallback(GLFWwindow* window, int key, int action, int mods)
{
	if(action == GLFW_PRESS && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		Raycast::onMouseClick(Engine::GetInstance().camera.getProjMat(), Engine::GetInstance().camera.getViewMat());
	}
}
