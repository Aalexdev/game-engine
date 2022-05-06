#pragma once
#include "Window.hpp"
#include "../renderer/Context.hpp"

#include "../Events/inputs/GLFWInput.hpp"

// GLFW prototype
struct GLFWwindow;

namespace engine::windows{
	class ENGINE_API GLFWWindow : public Window{
		public:

			GLFWWindow(const Properties &properties);
			~GLFWWindow();

			virtual uint32_t getWidth() const override;
			virtual uint32_t getHeight() const override;

			virtual void setEventCallback(const EventCallbackFn& callback) override;
            virtual void setVSync(bool enable) override;
            virtual bool isVSyncEnable() const override;

            virtual void* getNativeWindow() const override;
            virtual float getTime() const override;
            virtual float getAspectRatio() const override;

			virtual void setTitle(const std::string &title) override;


			virtual void update() override;

			virtual Ref<Context> getContext() const override {return context;}
			virtual Ref<Input> getInputs() const override {return data.inputs;}

		private:
			void initWindow(const Properties &properties);
			void shutdown();

			GLFWwindow *window;
			Ref<Context> context;

			struct WindowData{
                uint32_t width, height;
                std::string title;
                bool VSync;
                EventCallbackFn EventCallback;
			Ref<inputs::GLFWInput> inputs;
            };

			WindowData data; 

			static void initGLFW();
			static void shutdownGLFW();
	};
}