#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>


class IBaseHandler
{
public:
	virtual void AddComponent(unsigned int entityID) = 0;
	virtual bool HasComponent(unsigned int entityID) = 0;
};

template<typename Type>
class ComponentHandler : public IBaseHandler
{
public:
	void AddComponent(unsigned int entityID) override
	{
		if(entityToIndex.contains(entityID))
			return;
		entityToIndex[entityID] = components.size();
		components.emplace_back();
	}
	bool HasComponent(unsigned int entityID) override
	{
		return entityToIndex.contains(entityID);
	}
	Type& GetComponent(unsigned int entityID)
	{
		if (!entityToIndex.contains(entityID))
			throw;
		return components[entityToIndex[entityID]];
	}

private:
	std::unordered_map<unsigned, size_t> entityToIndex;
	std::vector<Type> components;
};

class ComponentManager
{
public:
	static ComponentManager& GetInstance();
	template<typename Type>
	void registerType()
	{
		if(componentHandlers.contains(typeid(Type)))
			return;
		componentHandlers[typeid(Type)] = std::make_unique<ComponentHandler<Type>>();
	}
	template<typename ...Components>
	void addComponents(unsigned int entityID)
	{
		if (!(componentHandlers.contains(typeid(Components))&&...))
			return;
		(componentHandlers[typeid(Components)]->AddComponent(entityID),...);
	}

	template<typename ...Components>
	bool HasComponents(unsigned int entityID)
	{
		return (componentHandlers.contains(typeid(Components))&&...&&(componentHandlers[typeid(Components)]->HasComponent(entityID)&&...));
	}

	template<typename Type>
	Type& GetComponent(unsigned int entityID)
	{
		if (!componentHandlers.contains(typeid(Type)))
			throw;
		return dynamic_cast<ComponentHandler<Type>*>(componentHandlers[typeid(Type)].get())->GetComponent(entityID);
	}
private:
	ComponentManager();
	std::unordered_map<std::type_index, std::unique_ptr<IBaseHandler>> componentHandlers;
};


