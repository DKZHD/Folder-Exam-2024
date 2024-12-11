#include "Raycast.h"
#include "GLFW/glfw3.h"
#include "src/Engine.h"
#include "glm/matrix.hpp"
#include "glm/vec4.hpp"
#include "src/ECS/ComponentManager/ComponentManager.h"
#include "src/ECS/Components/Components.h"
#include "src/ECS/EntityManager/EntityManager.h"
#include "src/ECS/Systems/Systems.h"

glm::vec2 Raycast::ConvertMousePosToNDC()
{
	double x, y;
	int width, height;
	glfwGetCursorPos(Engine::GetInstance().GetWindow(),&x,&y);
	glfwGetWindowSize(Engine::GetInstance().GetWindow(), &width, &height);
	return { (2.f * x) / width - 1.f, 1.f - (2.f * y) / height };
}

glm::vec3 Raycast::GetRayDirection(const glm::mat4& projection, const glm::mat4& view)
{
	glm::vec2 NDC = ConvertMousePosToNDC();
	glm::mat4 inverse = glm::inverse(projection * view);
	glm::vec4 near = glm::vec4(NDC,-1.f,1.f);
	glm::vec4 far = glm::vec4(NDC,1.f,1.f);

	glm::vec4 worldNear = inverse * near;
	glm::vec4 worldFar = inverse * far;

	glm::vec3 rayStart = glm::vec3(worldNear) / worldNear.w;
	glm::vec3 rayEnd = glm::vec3(worldFar) / worldFar.w;

	return glm::normalize(rayEnd - rayStart);
}

bool Raycast::RaycastIntersection(const glm::vec3& rayStart, const glm::vec3& rayDir, const AABB& bounding_box,
	float& near)
{
	glm::vec3 invDir = 1.f / rayDir;
	glm::vec3 tMin = (bounding_box.min - rayStart) * invDir;
	glm::vec3 tMax = (bounding_box.max - rayStart) * invDir;

	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm::max(tMin, tMax);

	near = glm::max(glm::max(t1.x, t1.y), t1.z);
	float far = glm::min(glm::min(t2.x, t2.y), t2.z);

	return near <= far && far >= 0.f;
}

int Raycast::GetEntityByRaycast(const glm::vec3& rayStart, const glm::vec3& rayDir)
{
	float closestT = std::numeric_limits<float>::max();
	int closestObject = -1;

	CollisionSystem::updateBoundaries();

	for (unsigned id = 0; id < Entity::globalID;id++) 
	{
		float tNear;
		if (RaycastIntersection(rayStart, rayDir, ComponentManager::GetHandler<CollisionComponent>().GetComponent(id).boundingBox, tNear)) {
			if (tNear < closestT) {
				closestT = tNear;
				closestObject = id;
			}
		}
	}
	return closestObject;
}

void Raycast::onMouseClick(const glm::mat4& projection, const glm::mat4& view)
{
	glm::vec3 rayDir = GetRayDirection(projection,view);
	glm::vec3 rayStart = glm::vec3(glm::inverse(view)[3]);

	ActiveID = GetEntityByRaycast(rayStart, rayDir);
}

int Raycast::GetActiveID()
{
	return ActiveID;
}

