#pragma once
#include <string>

#include "glm/fwd.hpp"

class Shader
{
public:
	void loadShader(const char* vertPath, const char* fragPath);

	void Use();
	unsigned GetProgram() const;

	// Uniform Setters
	void SetVec3(const char* name, glm::vec3 v) const;
	void SetVec4(const char* name, glm::vec4 v) const;
	void SetBool(const char* name, bool b) const;
	void SetInt(const char* name, int i) const;
	void SetFloat(const char* name, float f) const;
	void SetMat4(const char* name, glm::mat4 m) const;

private:
	std::string readShaderFile(const char* path) const;
	uint32_t Program = 0;
};
