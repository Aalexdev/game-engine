#pragma once

#include "Event.hpp"

namespace engine{
    class ENGINE_API MouseMovedEvent : public Event{
        public:
            MouseMovedEvent(Application *app, float x, float y) : Event(app), mouseX{x}, mouseY{y} {}

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
            MouseScrolledEvent(Application *app, float xOffset, float yOffset) : Event(app), xOffset{xOffset}, yOffset{yOffset} {}

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
            MouseButtonEvent(Application *app, int button) : Event(app), button{button} {}

            int button;
    };

    class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent{
        public:
            MouseButtonPressedEvent(Application *app, int button, bool doubleClicked) : MouseButtonEvent(app, button), doubleClicked{doubleClicked} {}

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
            MouseButtonReleasedEvent(Application *app, int button) : MouseButtonEvent(app, button) {}

            std::string toString() const override{
                std::stringstream ss;
                ss << "MouseButtonReleasedEvent : " << button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(EVENT_MOUSE_BUTTON_RELEASED);

    };
}