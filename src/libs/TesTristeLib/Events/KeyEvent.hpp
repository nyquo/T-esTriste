#pragma once

#include "Event.hpp"

#include <TesTristeLib/TesTristeExport.hpp>

#ifndef NDEBUG
#include <sstream>
#endif

namespace TesTriste {

class TET_API KeyEvent : public Event {
  public:
    using KeyCode = unsigned int;

    KeyCode getKeyCode() const { return m_keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyEvent)

  protected:
    KeyEvent(const KeyCode keyCode)
      : m_keyCode(keyCode) {}

    KeyCode m_keyCode;
};

class TET_API KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(const KeyCode keyCode, bool isRepeat = false)
      : KeyEvent(keyCode)
      , m_isRepeat(isRepeat) {}

    bool isRepeat() const { return m_isRepeat; }

    EVENT_CLASS_TYPE(KeyPressedEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " Key Pressed: " << m_keyCode << " (repeat = " << m_isRepeat << ")";
        return ss.str();
    }
#endif

  private:
    bool m_isRepeat;
};

class TET_API KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(KeyCode keyCode)
      : KeyEvent(keyCode) {}

    EVENT_CLASS_TYPE(KeyReleasedEvent)

#ifndef NDEBUG
    std::string toString() const override {
        std::stringstream ss;
        ss << getEventName() << " Key Released: " << m_keyCode;
        return ss.str();
    }
#endif
};

}
