#pragma once

#include <TesTristeLib/Core/Logger.hpp>

#include <array>
#include <bitset>
#include <limits>
#include <memory>
#include <optional>
#include <queue>
#include <stddef.h>
#include <testristelib_export.h>
#include <unordered_map>
#include <vector>

/**
 * Inspired by the work of:
 * - https://skypjack.github.io/
 * - https://austinmorlan.com/posts/entity_component_system/
 * - https://www.youtube.com/@cient_dev
 * For now a very simple implementation of an Entity Component System (ECS).
 * This is not a full implementation, but a starting point for further development.
 * Maybe this will replace EnTT later
 */

namespace TesTriste {

using Entity = unsigned int;
using ComponentId = size_t;
static constexpr Entity NULL_ENTITY = std::numeric_limits<Entity>::max();

class TET_EXPORT Component {
    static ComponentId identifier() noexcept {
        static ComponentId value = 0;
        return value++;
    }

  public:
    template<typename>
    static ComponentId type() noexcept {
        static const ComponentId value = identifier();
        return value;
    }
};

class TET_EXPORT IComponentPool {
  public:
    virtual ~IComponentPool() = default;
};

template<typename T>
class TET_EXPORT ComponentPool : IComponentPool {
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
    const std::vector<T>& getAll() const;

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
    ~ECS();

    Entity createEntity();
    void destroyEntity(Entity entity);
    size_t getEntityCount() const noexcept;

    template<typename T>
    void registerComponent();

    template<typename T>
    void addComponent(Entity entity, T&& component);

    template<typename T>
    void removeComponent(Entity entity);

    template<typename... T>
    bool hasComponent(Entity entity) const;

    template<typename T>
    T& getComponent(Entity entity);

    template<typename T>
    const T& getComponent(Entity entity) const;

    template<typename T>
    std::vector<T>& getAllComponent();

    template<typename T>
    const std::vector<T>& getAllComponent() const;

  private:
    std::queue<Entity> m_availableEntities;
    std::array<Signature, MAX_ENTITIES> m_signatures;
    std::unordered_map<ComponentId, IComponentPool*> m_componentPools;
};

template<typename T>
inline void ECS::registerComponent() {
    if(m_componentPools.size() >= MAX_COMPONENTS) {
        Logger::logError("Maximum number of components exceeded!");
        return;
    }
    if(m_componentPools.contains(Component::type<T>())) {
        Logger::logWarning("Component already registered: ", Component::type<T>());
        return;
    }
    m_componentPools[Component::type<T>()] = new ComponentPool<T>(MAX_ENTITIES);
}

template<typename T>
inline void ECS::addComponent(Entity entity, T&& component) {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    }
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        return;
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    pool->set(entity, std::move(component));
    m_signatures[entity].set(Component::type<T>());
}

template<typename T>
inline void ECS::removeComponent(Entity entity) {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        return;
    }
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        return;
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    pool->remove(entity);
    m_signatures[entity].reset(Component::type<T>());
}

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
inline T& ECS::getComponent(Entity entity) {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        throw std::out_of_range("Invalid entity ID");
    }
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        throw std::runtime_error("Component not registered");
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    return pool->get(entity);
}

template<typename T>
inline const T& ECS::getComponent(Entity entity) const {
    if(entity >= MAX_ENTITIES) {
        Logger::logError("Invalid entity ID: ", entity);
        throw std::out_of_range("Invalid entity ID");
    }
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        throw std::runtime_error("Component not registered");
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    return pool->get(entity);
}

template<typename T>
inline std::vector<T>& ECS::getAllComponent() {
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        return {};
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    return pool->getAll();
}

template<typename T>
inline const std::vector<T>& ECS::getAllComponent() const {
    if(!m_componentPools.contains(Component::type<T>())) {
        Logger::logError("Component not registered: ", Component::type<T>());
        return {};
    }
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[Component::type<T>()]);
    return pool->getAll();
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
    m_denseStorage[index] = std::move(m_denseStorage.back()); // Move might not be right here
    m_denseToSparse[index] = m_denseToSparse[m_denseStorage.size() - 1];
    m_denseStorage.pop_back();
    m_sparseStorage[m_denseToSparse[index]] = index;
    m_sparseStorage[entity] = NULL_ENTITY;
    m_denseToSparse[m_denseStorage.size()] = NULL_ENTITY;
}

template<typename T>
inline std::vector<T>& ComponentPool<T>::getAll() {
    return m_denseStorage;
}
template<typename T>
inline const std::vector<T>& ComponentPool<T>::getAll() const {
    return m_denseStorage;
}
}