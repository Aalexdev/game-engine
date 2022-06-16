#pragma once

#include "Display.hpp"
#include "../Events/inputs/SDL2Input.hpp"
#include "../renderer/Renderer.hpp"

struct SDL_Window;

namespace engine::displays{
	class ENGINE_API SDL2_Display : public Display{
		public:
			static Ref<SDL2_Display> create(const Definition &properties);

			SDL2_Display(const Definition &properties);
			~SDL2_Display();

			virtual glm::u32vec2 getSize() const override;
			virtual glm::u32vec2 getMinimalSize() const override;
			virtual glm::u32vec2 getMaximalSize() const override;
			virtual glm::u32vec2 getPosition() const override;
			virtual uint8_t getMonitorIndex() const override;
			virtual float getOpacity() const override;
			virtual bool isFullscreen() const override;
			virtual bool isBordered() const override;
			virtual const std::string &getTitle() const override;

			virtual void resize(const glm::u32vec2 &size) override;
			virtual void setMinimalSize(const glm::u32vec2 &size) override;
			virtual void setMaximalSize(const glm::u32vec2 &size) override;
			virtual void move(const glm::u32vec2 &position) override;
			virtual void setMonitor(uint8_t monitorIndex) override;
			virtual void setTitle(const std::string &title) override;
			virtual void setEventCallback(const EventFn &callback) override;

			virtual void* getNativeWindow() const override;
			virtual float getTime() const override;
			virtual float getAspectRatio() const override;

			virtual void update() override;

		private:

			void *window;
			Definition windowData;

			void onWindowEvent(void *event);
			void onWindowClosedEvent(void *event);
			void onWindowFocusedEvent(void *event);
			void onWindowLostFocusEvent(void *event);
			void onWindowMovedEvent(void *event);
			void onWindowMinimizedEvent(void *event);
			void onWindowMaximizedEvent(void *event);
			void onWindowResizedEvent(void *event);
			void onKeyDownEvent(void *event);
			void onKeyUpEvent(void *event);
			void onMouseButtonDownEvent(void *event);
			void onMouseButtonUpEvent(void *event);
			void onMouseMovedEvent(void *event);
			void onMouseWheelEvent(void *event);
	};
}