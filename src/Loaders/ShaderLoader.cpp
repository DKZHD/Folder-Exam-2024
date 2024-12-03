#include "ShaderLoader.h"
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "glm/matrix.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

void Shader::loadShader(const char* vertPath, const char* fragPath)
{
	std::string vertSrc = readShaderFile(vertPath);
	std::string fragSrc = readShaderFile(fragPath);

	const char* vertSrcChar = vertSrc.c_str();
	const char* fragSrcChar = fragSrc.c_str();

	int success;
	char buff[512];

	Program = glCreateProgram();
	uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertSrcChar, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs,GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vs, sizeof(buff), nullptr, buff);
		std::printf("\033[31mVertex Compilation Error:\n%s\033[0m",buff);
	}

	uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragSrcChar, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, sizeof(buff), nullptr, buff);
		std::printf("\033[31mFragment Compilation Error:\n%s\033[0m", buff);
	}

	glAttachShader(Program, vs);
	glAttachShader(Program, fs);
	glLinkProgram(Program);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::Use()
{
	glUseProgram(Program);
}

uint32_t Shader::GetProgram() const
{
	return Program;
}

void Shader::SetVec3(const char* name, glm::vec3 v) const
{
	glUniform3fv(glGetUniformLocation(Program, name), 1, &v[0]);
}

void Shader::SetVec4(const char* name, glm::vec4 v) const
{
	glUniform4fv(glGetUniformLocation(Program, name), 1, &v[0]);
}

void Shader::SetBool(const char* name, bool b) const
{
	glUniform1i(glGetUniformLocation(Program, name),b);
}

void Shader::SetInt(const char* name, int i) const
{
	glUniform1i(glGetUniformLocation(Program, name), i);
}

void Shader::SetFloat(const char* name, float f) const
{
	glUniform1f(glGetUniformLocation(Program, name), f);
}

void Shader::SetMat4(const char* name, glm::mat4 m) const
{
	glUniformMatrix4fv(glGetUniformLocation(Program, name), 1,GL_FALSE,&m[0][0]);
}

std::string Shader::readShaderFile(const char* path) const
{
	std::ifstream infile(path);
	std::stringstream sstream;
	sstream << infile.rdbuf();
	return sstream.str();
}
