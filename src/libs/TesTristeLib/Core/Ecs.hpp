#pragma once

#include <TesTristeLib/Core/Logger.hpp>

#include <array>
#include <bitset>
#include <limits>
#include <optional>
#include <queue>
#include <stddef.h>
#include <testristelib_export.h>
#include <vector>

/**
 * Inspired by the work of:
 * - https://skypjack.github.io/
 * - https://austinmorlan.com/posts/entity_component_system/
 * - https://www.youtube.com/@cient_dev
 * For now a very simple implementation of an Entity Component System (ECS).
 * This is not a full implementation, but a starting point for further development.
 * Why not use an existing ECS library? I want to learn how an ECS works and how to implement it.
 */

namespace TesTriste {

using Entity = unsigned int;
static constexpr Entity NULL_ENTITY = std::numeric_limits<Entity>::max();

class TET_EXPORT Component {
    static size_t identifier() noexcept {
        static size_t value = 0;
        return value++;
    }

  public:
    template<typename>
    static size_t type() noexcept {
        static const size_t value = identifier();
        return value;
    }
};

template<typename T>
class TET_EXPORT ComponentPool {
  public:
    ComponentPool(size_t maxEntities = 1000);
    ComponentPool(const ComponentPool& other) = delete;
    ComponentPool(ComponentPool&& other) = default;
    ComponentPool& operator=(const ComponentPool& other) = delete;
    ComponentPool& operator=(ComponentPool&& other) = default;
    ~ComponentPool() = default;

    void set(Entity entity, T&& component);
    T& get(Entity entity);
    const T& get(Entity entity) const;
    void remove(Entity entity);
    std::vector<T>& getAll();

  private:
    std::vector<T> m_denseStorage;
    std::vector<size_t> m_sparseStorage;
    std::vector<Entity> m_denseToSparse;
    size_t m_maxEntities;
};

class TET_EXPORT ECS {
  public:
    constexpr static size_t MAX_ENTITIES = 1000;
    constexpr static size_t MAX_COMPONENTS = 64;
    using Signature = std::bitset<MAX_COMPONENTS>;

    ECS();
    ECS(const ECS& other) = delete;
    ECS(ECS&& other) = delete;
    ECS& operator=(const ECS& other) = delete;
    ECS& operator=(ECS&& other) = delete;
    ~ECS() = default;

    Entity createEntity();
    void destroyEntity(Entity entity);
    size_t getEntityCount() const noexcept;

    template<typename T>
    void addComponent(Entity entity, T&& component);

    template<typename T>
    void removeComponent(Entity entity);

    template<typename... T>
    bool hasComponent(Entity entity) const;

  private:
    std::queue<Entity> m_availableEntities;
    std::array<Signature, MAX_ENTITIES> m_signatures;
};

template<typename T>
inline void ECS::addComponent(Entity entity, T&& component) {}

template<typename T>
inline void ECS::removeComponent(Entity entity) {}

template<typename... T>
inline bool ECS::hasComponent(Entity entity) const {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        return false;
    }
    Signature required;
    (required.set(Component::type<T>()), ...);
    return (m_signatures[entity] & required) == required;
}

template<typename T>
inline ComponentPool<T>::ComponentPool(size_t maxEntities)
  : m_sparseStorage(maxEntities, NULL_ENTITY)
  , m_maxEntities(maxEntities) {
    m_denseToSparse.reserve(maxEntities);
}

template<typename T>
inline void ComponentPool<T>::set(Entity entity, T&& component) {
    if(entity >= m_maxEntities) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    } else {
        if(m_sparseStorage[entity] == NULL_ENTITY) {
            m_sparseStorage[entity] = m_denseStorage.size();
            m_denseToSparse[m_denseStorage.size()] = entity;
            m_denseStorage.push_back(std::move(component));
        } else {
            m_denseStorage[m_sparseStorage[entity]] = std::move(component);
        }
    }
}

template<typename T>
inline T& ComponentPool<T>::get(Entity entity) {
    if(entity >= m_maxEntities) {
        Logger::logError("Invalid entity ID: ", entity);
        throw std::out_of_range("Invalid entity ID");
    } else if(m_sparseStorage[entity] == NULL_ENTITY) {
        Logger::logError("Entity does not have the component: ", entity);
        throw std::runtime_error("Entity does not have the component");
    }
    return m_denseStorage[m_sparseStorage[entity]];
}

template<typename T>
inline const T& ComponentPool<T>::get(Entity entity) const {
    if(entity >= m_maxEntities) {
        Logger::logError("Invalid entity ID: ", entity);
        throw std::out_of_range("Invalid entity ID");
    } else if(m_sparseStorage[entity] == NULL_ENTITY) {
        Logger::logError("Entity does not have the component: ", entity);
        throw std::runtime_error("Entity does not have the component");
    }
    return m_denseStorage[m_sparseStorage[entity]];
}

template<typename T>
inline void ComponentPool<T>::remove(Entity entity) {
    if(entity >= m_maxEntities) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    } else if(m_sparseStorage[entity] == NULL_ENTITY) {
        Logger::logError("Entity does not have the component: ", entity);
        return;
    }
    size_t index = m_sparseStorage[entity];
    m_denseStorage[index] = std::move(m_denseStorage.back());
    m_denseToSparse[index] = m_denseToSparse[m_denseStorage.size() - 1];
    m_denseStorage.pop_back();
    m_sparseStorage[entity] = NULL_ENTITY;

    // Update the sparse storage for the moved component
    if(!m_denseStorage.empty()) {
        m_sparseStorage[m_denseToSparse[m_denseStorage.size()]] = index;
        m_denseToSparse[m_denseStorage.size()] = NULL_ENTITY;
    }
}

template<typename T>
inline std::vector<T>& ComponentPool<T>::getAll() {
    return m_denseStorage;
}
}