#include "Ecs.hpp"

namespace TesTriste {

ECS::ECS() {
    for(size_t i = 0; i < MAX_ENTITIES; ++i) {
        m_availableEntities.push(i);
    }
}

ECS::~ECS() {
    for(auto& [id, pool] : m_componentPools) {
        delete pool; // Clean up component pools
    }
}

Entity ECS::createEntity() {
    if(m_availableEntities.empty()) {
        Logger::logError("No more entities available!");
        return NULL_ENTITY;
    }
    Entity entity = m_availableEntities.front();
    m_availableEntities.pop();
    return entity;
}

void ECS::destroyEntity(Entity entity) {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    }
    m_signatures[entity].reset();
    m_availableEntities.push(entity);
    Logger::logDebug("Entity destroyed: ", entity);
}

size_t ECS::getEntityCount() const noexcept { return MAX_COMPONENTS - m_availableEntities.size(); }
}
