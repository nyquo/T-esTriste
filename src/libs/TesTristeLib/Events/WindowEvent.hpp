#pragma once

#include "Event.hpp"

#include <TesTristeLib/TesTristeExport.hpp>

#ifndef NDEBUG
#include <sstream>
#endif

namespace TesTriste {

class TET_API WindowCloseEvent : public Event {
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

class TET_API WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(unsigned int width, unsigned int height)
      : m_width(width)
      , m_height(height) {}

    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }

    EVENT_CLASS_CATEGORY(EventCategoryWindowEvent)
    EVENT_CLASS_TYPE(WindowResizeEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " width: " << m_width << " | height: " << m_height;
        return ss.str();
    }
#endif

  private:
    unsigned int m_width, m_height;
};

}
