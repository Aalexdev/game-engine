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
                unsigned int width, height;
                std::string title;
                bool VSync;
                EventCallbackFn EventCallback;
            };

			WindowData data;
			
			bool (*SDLEventsCallback)(const SDL_Event*) = nullptr;

			static void initSDL2();
			static void shutdownSDL2();  
	};
}