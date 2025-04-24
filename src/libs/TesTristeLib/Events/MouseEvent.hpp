#pragma once

#include "Event.hpp"

#include <TesTristeLib/TesTristeExport.hpp>

#ifndef NDEBUG
#include <sstream>
#endif

namespace TesTriste {

class TET_API MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(double x, double y)
      : m_posX(x)
      , m_posY(y) {}

    double getX() const { return m_posX; }
    double getY() const { return m_posY; }

    EVENT_CLASS_CATEGORY(EventCategoryMouseEvent)
    EVENT_CLASS_TYPE(MouseMovedEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " x: " << m_posX << " | y: " << m_posY;
        return ss.str();
    }
#endif

  private:
    double m_posX, m_posY;
};

class TET_API MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(double xOffset, double yOffset)
      : m_xOffset(xOffset)
      , m_yOffset(yOffset) {}

    double getXOffset() const { return m_xOffset; }
    double getYOffset() const { return m_yOffset; }

    EVENT_CLASS_CATEGORY(EventCategoryMouseEvent)
    EVENT_CLASS_TYPE(MouseScrolledEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " xOffset: " << m_xOffset << " | yOffset: " << m_yOffset;
        return ss.str();
    }
#endif

  private:
    double m_xOffset, m_yOffset;
};

class TET_API MouseButtonEvent : public Event {
  public:
    using ButtonCode = int;

    ButtonCode getButtonCode() const { return m_buttonCode; }

    EVENT_CLASS_CATEGORY(EventCategoryMouseEvent)

  protected:
    MouseButtonEvent(const ButtonCode buttonCode)
      : m_buttonCode(buttonCode) {}

    ButtonCode m_buttonCode;
};

class TET_API MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(const ButtonCode buttonCode)
      : MouseButtonEvent(buttonCode) {}

    EVENT_CLASS_TYPE(MouseButtonPressedEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " Mouse Button Pressed: " << m_buttonCode;
        return ss.str();
    }
#endif
};

class TET_API MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(const ButtonCode buttonCode)
      : MouseButtonEvent(buttonCode) {}

    EVENT_CLASS_TYPE(MouseButtonReleasedEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " Mouse Button Released: " << m_buttonCode;
        return ss.str();
    }
#endif
};

}
