#include "EntityManager.h"

void EntityManager::update()
{
	// TODO: add entities from m_entitiesToAdd to the proper location(s)
	//		 - add them to the vector of all entities
	//       - add them to the vector inside the map, with the tag as a key
	for (auto entity : m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entityMap[entity->tag()].push_back(entity);
	}
	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	// C++20 way of iterating through [key,value] pairs in a map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// TODO: remove all dead entities from the input vector
	//		 this is called by the update() function
	// NOTE: This is the place we deal with iterator invalidation

	// BRO WTF IS THIS EVEN DOING BRO? look up Erase-Remove Idiom!!!
	vec.erase(
		std::remove_if(
			vec.begin(),
			vec.end(),
			[](auto entity) { return !entity->isActive(); }
		),
		vec.end()
	);
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: this is incorrect, return the correct vector from the map
	//return m_entities;

	return m_entityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_entityMap;
}