#pragma once

#include "Event.hpp"
#include "TesTristeLib/Core/Types.hpp"

#include <testristelib_export.h>

#ifndef NDEBUG
#include <sstream>
#endif

namespace TesTriste {

class TET_EXPORT WindowCloseEvent : public Event {
  public:
    EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
    EVENT_CLASS_TYPE(WindowCloseEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName();
        return ss.str();
    }
#endif
};

class TET_EXPORT WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(Size size)
      : m_size(size) {}

    Size getSize() const { return m_size; }

    EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
    EVENT_CLASS_TYPE(WindowResizeEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " width: " << m_size.width << " | height: " << m_size.height;
        return ss.str();
    }
#endif

  private:
    Size m_size;
};

}
