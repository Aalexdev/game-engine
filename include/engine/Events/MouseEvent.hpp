#pragma once

#include "Event.hpp"

namespace engine{
    class ENGINE_API MouseMovedEvent : public Event{
        public:
            MouseMovedEvent(float x, float y) : mouseX{x}, mouseY{y} {}

            inline float getX() const {return mouseX;}
            inline float getY() const {return mouseY;}

            std::string toString() const override{
                std::stringstream ss;
                ss << "MouseMovedEvent : " << mouseX << ", " << mouseY;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_MOUSE_MOVED);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);
        
        private:
            float mouseX, mouseY;
    };

    class ENGINE_API MouseScrolledEvent : public Event{
        public:
            MouseScrolledEvent(float xOffset, float yOffset) : xOffset{xOffset}, yOffset{yOffset} {}

            inline float getXOffset() const {return xOffset;}
            inline float getYOffset() const {return yOffset;}

            std::string toString() const override{
                std::stringstream ss;
                ss << "MouseScrolledEvent : " << xOffset << ", " << yOffset;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_MOUSE_SCROLLED);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT);

        private:
            float xOffset, yOffset;
    };

    class ENGINE_API MouseButtonEvent : public Event{
        public:
            inline int getMouseButton() const {return button;}
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT);
        protected:
            MouseButtonEvent(int button) : button{button} {}

            int button;
    };

    class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent{
        public:
            MouseButtonPressedEvent(int button, bool doubleClicked) : MouseButtonEvent(button), doubleClicked{doubleClicked} {}

            std::string toString() const override{
                std::stringstream ss;
                ss << "MouseButtonPressedEvent : " << button;
                return ss.str();
            }

			inline bool isDoubleClicked() const {return doubleClicked;}

            EVENT_CLASS_TYPE(EVENT_MOUSE_BUTTON_PRESSED);
		
		private:
			bool doubleClicked = false;
    };

    class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent{
        public:
            MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

            std::string toString() const override{
                std::stringstream ss;
                ss << "MouseButtonReleasedEvent : " << button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_MOUSE_BUTTON_RELEASED);

    };
}