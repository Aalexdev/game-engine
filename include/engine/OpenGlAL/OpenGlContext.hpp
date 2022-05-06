#pragma once

#include "../core.hpp"
#include "../renderer/Context.hpp"
#include "../windows/Window.hpp"

struct GLFWwindow;
struct SDL_Window;

namespace engine::OpenGl{
	class ENGINE_API OpenGLContext : public Context{
		public:
			virtual ~OpenGLContext(){}

			static Ref<OpenGLContext> create(Window *window);
	};

	class ENGINE_API GLFWOpenGLContext : public OpenGLContext{
		public:
			GLFWOpenGLContext(GLFWwindow *window);
			~GLFWOpenGLContext();

			virtual void init() override;
			virtual void swapBuffers() override;
			virtual void enableVSync(bool enable) override;

		private:
			GLFWwindow *window;
	};

	class ENGINE_API SDL2OpenGLContext : public OpenGLContext{
		public:
			SDL2OpenGLContext(SDL_Window *window);
			~SDL2OpenGLContext();

			virtual void init() override;
			virtual void swapBuffers() override;
			virtual void enableVSync(bool enable) override;

			void *getContext() const {return context;}
		
		private:
			SDL_Window *window;
			void *context;
	};
}