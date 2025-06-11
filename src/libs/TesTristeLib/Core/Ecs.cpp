#include "Ecs.hpp"

namespace TesTriste {

ECS::ECS() {
    for(size_t i = 0; i < MAX_ENTITIES; ++i) {
        m_availableEntities.push(i);
    }
}

Entity ECS::createEntity() {
    if(m_availableEntities.empty()) {
        Logger::logError("No more entities available!");
        return MAX_ENTITIES; // Return an invalid entity
    } else {
        Entity entity = m_availableEntities.front();
        m_availableEntities.pop();
        return entity;
    }
}

void ECS::destroyEntity(Entity entity) {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    }
    m_signatures[entity].reset();
    m_availableEntities.push(entity);
    Logger::logDebug("Entity destroyed: ", entity);

    // TODO clear components associated with the entity
}

size_t ECS::getEntityCount() const noexcept { return MAX_COMPONENTS - m_availableEntities.size(); }
}
