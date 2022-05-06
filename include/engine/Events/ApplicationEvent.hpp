#pragma once

#include "../pch.hpp"
#include "Event.hpp"

namespace engine{
    class ENGINE_API WindowResizedEvent : public Event{
        public:
            WindowResizedEvent(unsigned int width, unsigned int height) : width{width}, height{height} {}

            inline unsigned int getWidth() const {return width;}
            inline unsigned int getHeight() const {return height;}

            EVENT_CLASS_TYPE(EVENT_WINDOW_RESIZED);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);

            std::string toString() const override{
                std::stringstream ss;
                ss << "WindowResizedEvent : " << width << ", " << height;
                return ss.str();
            }

        private:
            unsigned int width, height;
    };

    class ENGINE_API WindowCloseEvent : public Event{
        public:
            WindowCloseEvent() {}

            EVENT_CLASS_TYPE(EVENT_WINDOW_CLOSED);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    };

    class ENGINE_API AppTickEvent : public Event{
        public:
            AppTickEvent() {}
            EVENT_CLASS_TYPE(EVENT_APP_TICK);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    };

    class ENGINE_API AppUpdateEvent : public Event{
        public:
            AppUpdateEvent();
            EVENT_CLASS_TYPE(EVENT_APP_UPDATE);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    };

    class ENGINE_API AppRenderEvent : public Event{
        public:
            AppRenderEvent();
            EVENT_CLASS_TYPE(EVENT_APP_RENDER);
            EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION);
    };
}