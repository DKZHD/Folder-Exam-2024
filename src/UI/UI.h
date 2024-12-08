#pragma once

#include <string>
#include <map>
#include <unordered_set>
#include <vector>

#include "glm/vec3.hpp"

struct GLFWwindow;

struct dragFloatInfo
{
	std::string displayText;
	unsigned int id;
};

class UI
{
public:
	void init(GLFWwindow* window);
	void render();
	void AddDragFloat(std::string name, unsigned int id);
	//inline static std::map<std::string, unsigned int> dragFloats;
	inline static std::vector<dragFloatInfo> dragFloats;
	inline static std::unordered_set<std::string> stringSet;
};

