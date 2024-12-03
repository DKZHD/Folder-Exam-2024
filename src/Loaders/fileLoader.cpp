#include "fileLoader.h"
#include <fstream>
#include <sstream>
#include "glm/vec3.hpp"

void fileLoader::LoadFile(const char* path)
{
	std::ifstream inFile(path);
	if (!inFile.is_open())
		throw;
	bool first = true;

	while (!inFile.eof())
	{
		if (inFile.peek() == 'T' || first)
		{
			std::string s;
			std::getline(inFile, s);
			std::stringstream ss(s);
			int i;
			ss >> i;
			filePosition.reserve(i);
			first = false;
		}

		if (inFile.eof())
			break;
		std::string s;
		std::getline(inFile, s);
		std::stringstream ss(s);
		glm::vec3 p(0.f);
		ss >> p.x >> p.y >> p.z;
		filePosition.emplace_back(p);
	}
}
void fileLoader::WriteFile(const char* path, const std::vector<glm::vec3>& v)
{
	std::ofstream outFile(path);
	if (!outFile.is_open())
		throw;
	outFile << v.size() << '\n';
	for (const glm::vec3& vec3 : v)
	{
		outFile << vec3.x << " " << vec3.y << " " << vec3.z << '\n';
	}
	outFile.close();
}

void fileLoader::WriteBinaryFile(const char* path, std::vector<glm::vec3> v)
{
	std::ofstream outFile(path, std::ios::binary);
	if (!outFile.is_open())
		throw;
	size_t size = v.size();
	outFile.write(reinterpret_cast<const char*>(&size),sizeof(size_t));
	for(const auto& pos : v)
	{
		outFile.write(reinterpret_cast<const char*>(&pos), sizeof(glm::vec3));
	}
	outFile.close();
}


void fileLoader::ReadBinaryFile(const char* path)
{
	std::ifstream infile(path, std::ios::binary);
	if (!infile.is_open())
		throw;
	size_t size = 0;
	infile.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	filePosition.reserve(size);

	for(size_t i = 0; i < size; i++)
	{
		glm::vec3 pos(0.f);
		infile.read(reinterpret_cast<char*>(&pos), sizeof(glm::vec3));
		filePosition.emplace_back(pos);
	}
	infile.close();
}
