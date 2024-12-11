#pragma once
#include <memory>
#include <vector>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "glm/vec3.hpp"


template<typename Type>
class ComponentHandler 
{
public:
	void AddComponent(unsigned int entityID) 
	{
		if(idtoIndex.contains(entityID))
			return;
		idtoIndex[entityID] = components.size();
		components.emplace_back();
	}
	bool HasComponent(unsigned int entityID) 
	{
		return idtoIndex.contains(entityID);
	}
	Type& GetComponent(unsigned int entityID)
	{
		if (!idtoIndex.contains(entityID))
			throw;
		return components[idtoIndex[entityID]];
	}
	std::vector<Type> components;
private:
	std::unordered_map<unsigned, size_t> idtoIndex;
};

class ComponentManager
{
public:
	static ComponentManager& GetInstance();
	template<typename Type>
	static ComponentHandler<Type>& GetHandler()
	{
		static ComponentHandler<Type> compHandler;
		return compHandler;
	}
private:
	ComponentManager() = default;
};


