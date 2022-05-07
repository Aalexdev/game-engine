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
				uint32_t x, y;
                std::string title;
                bool VSync;
                EventCallbackFn EventCallback;
				Ref<inputs::GLFWInput> inputs;

				uint32_t widthBeforeFullscreen;
				uint32_t heightBeforeFullscreen;
				uint32_t xBeforeFullscreen;
				uint32_t yBeforeFullscreen;

            };

			WindowData data; 

			static void initGLFW();
			static void shutdownGLFW();
	};
}