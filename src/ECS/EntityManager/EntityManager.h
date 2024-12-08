#pragma once
#include <vector>

struct Entity
{
	unsigned ID;
	inline static unsigned globalID = 0;
	Entity() : ID(globalID++){}
};

class EntityManager
{
public:
	unsigned AddEntity();
	unsigned GetLastEntity();
	static EntityManager& GetInstance();
	Entity& GetEntity(size_t index);
private:
	std::vector<Entity> entities;
};

