#pragma once

#include <string>
#include <map>

#include "glm/vec3.hpp"

struct GLFWwindow;

class UI
{
public:
	void init(GLFWwindow* window);
	void render();
	void AddDragFloat(std::string name, glm::vec3& vec);
	inline static std::map<std::string, glm::vec3*> dragFloats;
};

