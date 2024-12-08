#pragma once
#include <vector>

#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct AABB;

class Raycast
{
public:
	static glm::vec2 ConvertMousePosToNDC();
	static glm::vec3 GetRayDirection(const glm::mat4& projection, const glm::mat4& view);
	static bool RaycastIntersection(const glm::vec3& rayStart, const glm::vec3& rayDir, const AABB& bounding_box, float& near);
	static int GetEntityByRaycast(const glm::vec3& rayStart, const glm::vec3& rayDir);
	static void onMouseClick(const glm::mat4& projection, const glm::mat4& view);
	static int GetActiveID();
private:
	inline static int ActiveID = -1;
};

