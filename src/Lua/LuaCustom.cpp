#include "LuaCustom.h"

#include <filesystem>

#include "../Engine.h"
#include "../ECS/EntityManager/EntityManager.h"
#include "../ECS/ComponentManager/ComponentManager.h"
#include "glm/vec3.hpp"
#include "../ECS/Components/Components.h"
#include "../ECS/Systems/Systems.h"
#include "../Scene/Scene.h"

sol::state& LuaCustom::GetState()
{
	return luaState;
}

void LuaCustom::init()
{
	luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

	luaState.new_usertype<glm::vec3>(
		"vec3",
		sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z
	);

	luaState["vec3"] = luaState["vec3"]["new"];

	luaState.set_function("GetHealth", &HealthSystem::GetHealth);
	luaState.set_function("SetHealth", &HealthSystem::SetHealth);
	luaState.set_function("AddHealth", &HealthSystem::AddHealth);
	luaState.set_function("DecreaseHealth", &HealthSystem::DecreaseHealth);

	luaState.set_function("SetPosition", &MovementSystem::SetPosition);
	luaState.set_function("UpdatePosition", &MovementSystem::UpdatePosition);
	luaState.set_function("AddVelocity", &MovementSystem::AddVelocity);
	luaState.set_function("DecreaseVelocity", &MovementSystem::DecreaseVelocity);

	luaState.new_usertype<EntityManager>(
		"EntityManager",
		sol::no_constructor,
		"AddEntity", &EntityManager::AddEntity,
		"GetLast", &EntityManager::GetLastEntity
	);

	luaState.new_usertype<ComponentManager>(
		"ComponentManager",
		sol::no_constructor,
		"addComponent", [this](ComponentManager& self, unsigned int entityID, const std::string& component)
		{
			if(component == "HealthComponent")
			{
				self.addComponents<HealthComponent>(entityID);
			}
			else if(component == "PositionComponent")
			{
				self.addComponents<PositionComponent>(entityID);
			}
			else if(component == "MovementComponent")
			{
				self.addComponents<MovementComponent>(entityID);
			}
			else if(component == "BufferComponent")
			{
				self.addComponents<BufferComponent>(entityID);
			}
			else if(component == "CollisionComponent")
			{
				self.addComponents<CollisionComponent>(entityID);
			}
		},
		"getComponent", [this](ComponentManager& self, unsigned int entityID, const std::string& component) -> sol::object
		{
			if (component == "HealthComponent") 
			{
				return sol::make_object(luaState, &self.GetComponent<HealthComponent>(entityID));
			}
			if (component == "PositionComponent") 
			{
				return sol::make_object(luaState, &self.GetComponent<PositionComponent>(entityID));
			}
			if (component == "MovementComponent") 
			{
				return sol::make_object(luaState, &self.GetComponent<MovementComponent>(entityID));
			}
			return sol::make_object(luaState, sol::nil);
		}
	);

	luaState.set_function("addUiFloats", [](std::string name, unsigned int id)
		{
			Engine::GetInstance().ui.AddDragFloat(name, id);
		});

	luaState.new_usertype<HealthComponent>(
		"HealthComponent",
		"health", &HealthComponent::health
	);
	luaState.new_usertype<PositionComponent>(
		"PositionComponent",
		"position", &PositionComponent::position
	);
	luaState.new_usertype<MovementComponent>(
		"MovementComponent",
		"velocity", &MovementComponent::velocity
	);
	luaState.new_usertype<CollisionComponent>(
		"CollisionComponent",
		"boundingBox", &CollisionComponent::boundingBox
	);

	luaState["ComponentManager"] = &ComponentManager::GetInstance();
	luaState["EntityManager"] = &EntityManager::GetInstance();

	luaState.set_function("GenerateMesh", [](unsigned int id, const std::string& type, glm::vec3 position, glm::vec3 scale)
		{
		MeshType meshType;

		if (type == "Cube") meshType = MeshType::Cube;
		else if (type == "Sphere") meshType = MeshType::Sphere;
		else throw std::runtime_error("Unknown MeshType: " + type);

		MeshSystem::GenerateMesh(id, meshType, position, scale);
		});
}

void LuaCustom::LoadInitialFIles()
{
	std::string folderPath  = "./scripts";
	for(const auto& file : std::filesystem::directory_iterator(folderPath))
	{
		if(file.is_regular_file() && file.path().extension()==".lua")
		{
			const std::string filePath = file.path().string();
			luaState.script_file(filePath);
		}
	}
}
