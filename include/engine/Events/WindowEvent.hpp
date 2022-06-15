#pragma once

#include "../pch.hpp"
#include "Event.hpp"
#include "../displays/Display.hpp"

namespace engine{

	class ENGINE_API WindowEvent : public Event{
		public:
			Display* getDisplay() const {return display;}
			EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APPLICATION | EVENT_CATEGORY_WINDOW);

		protected:
			WindowEvent(Application *app, Display *display) : Event(app), display{display}{}
			Display* display;
	};

	class ENGINE_API WindowResizedEvent : public WindowEvent{
		public:
			WindowResizedEvent(Application *app, Display *display, unsigned int width, unsigned int height) : WindowEvent(app, display), width{width}, height{height} {}

			inline unsigned int getWidth() const {return width;}
			inline unsigned int getHeight() const {return height;}

			EVENT_CLASS_TYPE(EVENT_WINDOW_RESIZED);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowResizedEvent : " << width << ", " << height;
				return ss.str();
			}

		private:
			unsigned int width, height;
	};

	class ENGINE_API WindowCloseEvent : public WindowEvent{
		public:
			WindowCloseEvent(Application *app, Display *display) : WindowEvent(app, display) {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_CLOSED);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowClosedEvent";
				return ss.str();
			}
	};

	class ENGINE_API WindowFocusedEvent : public WindowEvent{
		public:
			WindowFocusedEvent(Application *app, Display *display) : WindowEvent(app, display) {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_FOCUSED);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowFocusedEvent";
				return ss.str();
			}
	};

	class ENGINE_API WindowLostFocusEvent : public WindowEvent{
		public:
			WindowLostFocusEvent(Application *app, Display *display) : WindowEvent(app, display) {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_LOST_FOCUS);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowLostFocusEvent";
				return ss.str();
			}
	};

	
	class ENGINE_API WindowMinimizedEvent : public WindowEvent{
		public:
			WindowMinimizedEvent(Application *app, Display *display) : WindowEvent(app, display) {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_LOST_FOCUS);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowMinimizedEvent";
				return ss.str();
			}
	};

	
	class ENGINE_API WindowMaximizedEvent : public WindowEvent{
		public:
			WindowMaximizedEvent(Application *app, Display *display) : WindowEvent(app, display) {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_LOST_FOCUS);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowMaximizedEvent";
				return ss.str();
			}
	};

	class ENGINE_API WindowMovedEvent : public WindowEvent{
		public:
			WindowMovedEvent(Application *app, Display *display, const glm::u32vec2& pos, uint8_t monitor) : WindowEvent(app, display), pos{pos}, monitor{monitor} {}
			EVENT_CLASS_TYPE(EVENT_WINDOW_MOVED);

			std::string toString() const override{
				std::stringstream ss;
				ss << "WindowMovedEvent";
				return ss.str();
			}

			const glm::u32vec2& getPosition() const {return pos;}
			const uint8_t getMonitor() const {return monitor;}
		
		private:
			glm::u32vec2 pos;
			uint8_t monitor;

	};
}