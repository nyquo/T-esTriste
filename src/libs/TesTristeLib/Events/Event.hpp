#pragma once

#include <TesTristeLib/TesTristeExport.hpp>

#include <functional>
#include <type_traits>

#ifndef NDEBUG
#include <ostream>
#include <string>
#endif

namespace TesTriste {

enum class EventType {
    // Window events
    WindowCloseEvent,
    WindowResizeEvent,

    // Mouse events
    MouseMovedEvent,
    MouseScrolledEvent,
    MouseButtonPressedEvent,
    MouseButtonReleasedEvent,

    // Key events
    KeyPressedEvent,
    KeyReleasedEvent
};

enum class EventCategory : int {
    None = 0,
    EventCategoryWindowEvent = 1 << 1,
    EventCategoryMouseEvent = 1 << 2,
    EventCategoryKeyEvent = 1 << 3
};

// Override the type associated methods for an event
#ifndef NDEBUG
#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType getStaticType() { return EventType::type; }                                                       \
    virtual EventType getEventType() const override { return getStaticType(); }                                        \
    virtual const char* getEventName() const override { return #type; }
#else
#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType getStaticType() { return EventType::type; }                                                       \
    virtual EventType getEventType() const override { return getStaticType(); }
#endif

// Override the category associated methods for an event
#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual EventCategory getCategory() const override { return EventCategory::category; }

inline EventCategory operator|(EventCategory l, EventCategory r) {
    return static_cast<EventCategory>(static_cast<std::underlying_type_t<EventCategory>>(l) |
                                      static_cast<std::underlying_type_t<EventCategory>>(r));
}

class TET_API Event {
  public:
    virtual ~Event() = default;
    [[nodiscard]] virtual EventType getEventType() const = 0;
    [[nodiscard]] virtual EventCategory getCategory() const = 0;
    [[nodiscard]] inline bool isInCateory(EventCategory category) const {
        return static_cast<std::underlying_type_t<EventCategory>>(getCategory()) &
               static_cast<std::underlying_type_t<EventCategory>>(category);
    }
#ifndef NDEBUG
    virtual const char* getEventName() const = 0;
    virtual std::string toString() const { return getEventName(); }
#endif

    bool m_handled{ false };
};

#ifndef NDEBUG
inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.toString(); }
#endif

class TET_API EventDispatcher {
  public:
    EventDispatcher(Event& event)
      : m_event(event) {}
    EventDispatcher() = delete;

    template<typename T>
    void dispatch(std::function<bool(T&)> function) {
        if(m_event.getEventType() == T::getStaticType()) {
            m_event.m_handled |= function(*dynamic_cast<T*>(&m_event));
        }
    }

  private:
    Event& m_event;
};

#define BIND_EVENT_FN(e) std::bind(&e, this, std::placeholders::_1)

}