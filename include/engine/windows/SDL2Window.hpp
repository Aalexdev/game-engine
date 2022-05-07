#pragma once
#include "Window.hpp"
#include "../renderer/Context.hpp"
#include "../Events/inputs/SDL2Input.hpp"

#include <libs/SDL2/SDL.h>

namespace engine::windows{
	class ENGINE_API SDL2Window : public Window{
		public:
			SDL2Window(const Properties &properties);
			~SDL2Window();

			virtual uint32_t getWidth() const override;
			virtual uint32_t getHeight() const override;
			virtual uint32_t getX() const override;
			virtual uint32_t getY() const override;
			virtual bool isBorderless() const override;
			virtual bool isFullscreen() const override;
			virtual bool isGrab() const override;
			virtual float getOpacity() const override;
			virtual uint32_t getMinWidth() const override;
			virtual uint32_t getMaxWidth() const override;
			virtual uint32_t getMinHeight() const override;
			virtual uint32_t getMaxHeight() const override;

			virtual void setSize(uint32_t width, uint32_t height) override;
			virtual void setPos(uint32_t x, uint32_t y) override;
			virtual void setBorderless(bool borderless) override;
			virtual void setFullscreen(bool fullscreen) override;
			virtual void setGrab(bool grab) override;
			virtual void setOpacity(float opacity) override;
			virtual void setMinSize(uint32_t width, uint32_t height) override;
			virtual void setMaxSize(uint32_t width, uint32_t height) override;

			virtual void setEventCallback(const EventCallbackFn& callback) override;
            virtual void setVSync(bool enable) override;
            virtual bool isVSyncEnable() const override;

            virtual void* getNativeWindow() const override;
            virtual float getTime() const override;
            virtual float getAspectRatio() const override;

			virtual void update() override;

			virtual Ref<Context> getContext() const override {return context;}
			virtual Ref<Input> getInputs() const override {return inputs;}

			virtual void setTitle(const std::string &title) override;

			void setSDLEventCallback(bool (*callback)(const SDL_Event*)) {SDLEventsCallback = callback;}

		private:
			void initWindow(const Properties &properties);
			void shutdown();

			SDL_Window *window;
			Ref<Context> context;
			Ref<inputs::SDL2Input> inputs;

			struct WindowData{
                uint32_t width, height;
				uint32_t x, y;
                std::string title;
                bool VSync;
				bool borderless;
				bool fullscreen;
                EventCallbackFn EventCallback;
            };

			WindowData data;
			
			bool (*SDLEventsCallback)(const SDL_Event*) = nullptr;

			static void initSDL2();
			static void shutdownSDL2();  
	};
}