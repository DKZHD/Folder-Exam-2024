#pragma once
#include <vector>
#include "glm/fwd.hpp"

class Math
{
public:
	static glm::vec3 CalculateBSplineSurfacePoint(const std::vector<std::vector<glm::vec3>>& controlPoints, const std::vector<float>& knotVectorU, const std::vector<float>& knotVectorV, float u, float v, int degreeU, int degreeV);
	static std::vector<float> CalculateKnotVector(int amount_p, int degree);
	static std::vector<std::vector<glm::vec3>> Create2DVectorFrom1D(std::vector<glm::vec3> vector);
	static std::vector<glm::vec3> CalculateSurface(int samples, const std::vector<std::vector<glm::vec3>>& controlPoints);
	static float BasisFunction(int d, const std::vector<float>& knots, int i, float t);
	static std::pair<std::vector<glm::vec3>,int> CalculateChunks(std::vector<glm::vec3>& pointCloud, float ChunkSize);
};

