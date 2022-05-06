#pragma once
#include "Event.hpp"

namespace engine{
    class ENGINE_API KeyEvent : public Event{
        public:
            inline Key getKeyCode() const {return keyCode;}
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);

        protected:
            KeyEvent(Key keyCode) : keyCode{keyCode} {}
            Key keyCode;
    };

    class ENGINE_API KeyPressedEvent : public KeyEvent{
        public:
            KeyPressedEvent(Key keyCode, int repeatCount) : KeyEvent(keyCode), repeatCount{repeatCount} {}
            inline int getRepeatCount() const {return repeatCount;}

            std::string toString() const override {
                std::stringstream ss;
                ss << "KeyPressedEvent : " << KeyToStr(keyCode) << "(" << repeatCount << " repeats)";
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_KEY_PRESSED);

        private:
            int repeatCount;
    };

    class ENGINE_API KeyReleasedEvent : public KeyEvent{
        public:
            KeyReleasedEvent(Key keyCode) : KeyEvent(keyCode) {}

            std::string toString() const override{
                std::stringstream ss;
                ss << "KeyReleasedEvent : " << KeyToStr(keyCode);
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_KEY_RELEASED);
    };

    class ENGINE_API KeyTypedEvent : public Event{
        public:
            KeyTypedEvent(const std::string &text) : text{text}{}

            std::string toString() const override {
                std::stringstream ss;
                ss << "KeyTypedEvent : " << text;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_KEY_TYPED);
			EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT);
		
		private:
			std::string text;
    };
}