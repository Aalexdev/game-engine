#pragma once

#include "../pch.hpp"
#include "../core.hpp"

namespace engine{
    enum class EventType{
        EVENT_NONE = 0,
        EVENT_WINDOW_CLOSED,
        EVENT_WINDOW_RESIZED,
        EVENT_WINDOW_FOCUS,
        EVENT_WINDOW_LOST_FOCUS,
        EVENT_WINDOW_MOVED,
        EVENT_APP_TICK,
        EVENT_APP_UPDATE,
        EVENT_APP_RENDER,
        EVENT_KEY_PRESSED,
        EVENT_KEY_RELEASED,
        EVENT_KEY_TYPED,
        EVENT_MOUSE_BUTTON_PRESSED,
        EVENT_MOUSE_BUTTON_RELEASED,
        EVENT_MOUSE_MOVED,
        EVENT_MOUSE_SCROLLED
    };

    enum EventCategory{
        NONE = 0,
        EVENT_CATEGORY_APPLICATION    = BIT(0),
        EVENT_CATEGORY_INPUT          = BIT(1),
        EVENT_CATEGORY_KEYBOARD       = BIT(2),
        EVENT_CATEGORY_MOUSE          = BIT(3),
        EVENT_CATEGORY_MOUSE_BUTTON   = BIT(4)
    };

    #define EVENT_CLASS_TYPE(type)  static EventType getStaticType() {return EventType::type;} \
                                    virtual EventType getEventType() const override {return getStaticType();} \
                                    virtual const char * getName() const override {return #type;}
    
    #define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override {return category;}

    class ENGINE_API Event{
        friend class EventDispatcher;

        public:
            virtual EventType getEventType() const = 0;
            virtual const char *getName() const = 0;
            virtual int getCategoryFlags() const = 0;
            virtual std::string toString() const {return getName();}

            inline bool isInCategory(EventCategory category){
                return getCategoryFlags() & category;
            }

            inline bool isHandled() const {return handled;}
        
        private:
            bool handled = false;
    };

    class EventDispatcher{
        template<typename T>
        using EventFn = std::function<bool(T&)>;

        public:
            EventDispatcher(Event &event) : event{event} {}

            template<typename T>
            bool dispatch(EventFn<T> func){
                if (event.getEventType() == T::getStaticType()){
                    event.handled = func(*(T*)&event);
                    return true;
                }
                return false;
            }
        
        private:
            Event &event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event &e){
        return os << e.toString();
    }
}