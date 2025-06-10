#pragma once

#include <stddef.h>
#include <testristelib_export.h>

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

class TET_EXPORT Family {
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

}