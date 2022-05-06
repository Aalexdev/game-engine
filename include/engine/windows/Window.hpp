/**
 * @file Window.hpp
 * @author Aaledev (aaleex3984@gmail.com)
 * @brief the header file for the window abstraction layer
 * @version 0.1
 * @date 2022-03-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "../pch.hpp"
#include "../core.hpp"
#include "../Events/Event.hpp"
#include "../renderer/Context.hpp"
#include "../Events/Input.hpp"

namespace engine{
	class ENGINE_API Window{
		constexpr static uint32_t WINDOW_POS_UNDEFINED = UINT32_MAX;
		public:

			using EventCallbackFn = std::function<void(Event&)>;
			
			struct Properties{
				std::string title = "window";
				uint32_t width = 1080;
				uint32_t height = 720;
				
				Properties() {}
			};

			enum class WindowAPI{
				GLFW,
				SDL2
			};

			Window() {}
			virtual ~Window() {};

			// delete copy
			Window(const Window &) = delete;
			const Window& operator=(const Window &) = delete;

			/**
			 * @brief create a window 
			 * 
			 * @param properties the properties used to create the window (optional)
			 * @return Ref<Window> 
			 */
			static Ref<Window> create(const Properties &properties = Properties());

			static WindowAPI getWindowAPI() {return windowAPI;}
			static void setWindowAPI(WindowAPI API) {windowAPI = API;}

			// virtual functions

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;

			virtual void setEventCallback(const EventCallbackFn& callback) = 0;
			virtual void setVSync(bool enable) = 0;
			virtual bool isVSyncEnable() const = 0;

			virtual void* getNativeWindow() const = 0;
			virtual Ref<Context> getContext() const = 0;
			virtual Ref<Input> getInputs() const = 0;

			virtual float getTime() const = 0;
			virtual float getAspectRatio() const = 0;

			virtual void update() = 0;

			virtual void setTitle(const std::string &title) = 0;
		
		private:
			static WindowAPI windowAPI;

	};
} // namespace engine

