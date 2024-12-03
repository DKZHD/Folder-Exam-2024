#pragma once

#include <vector>
#include "glm/vec3.hpp"

class fileLoader
{
public:
	static inline std::vector<glm::vec3> filePosition;
	static void LoadFile(const char* path);
	static void WriteBinaryFile(const char* path, std::vector<glm::vec3> v);
	static void WriteFile(const char* path, const std::vector<glm::vec3>& v);
	static void ReadBinaryFile(const char* path);
};
