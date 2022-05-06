#include "engine/OpenGlAL/OpenGlContext.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include <libs/glad/glad.h>
#include <libs/GLFW/glfw3.h>
#include <libs/SDL2/SDL.h>

namespace engine::OpenGl{
	Ref<OpenGLContext> OpenGLContext::create(Window *window){
		ENGINE_PROFILE_FUNCTION();

		switch (Window::getWindowAPI()){
			case Window::WindowAPI::GLFW: return createRef<GLFWOpenGLContext>(static_cast<GLFWwindow*>(window->getNativeWindow()));
			case Window::WindowAPI::SDL2: return createRef<SDL2OpenGLContext>(static_cast<SDL_Window*>(window->getNativeWindow()));
		}
		ENGINE_CORE_ASSERT(false, "unknown window API");
		return nullptr;
	}

	// GLFW
	
	GLFWOpenGLContext::GLFWOpenGLContext(GLFWwindow *window) : window{window}{}
	GLFWOpenGLContext::~GLFWOpenGLContext(){}

	void GLFWOpenGLContext::init(){
		ENGINE_PROFILE_FUNCTION();
        
        glfwMakeContextCurrent(window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ENGINE_CORE_ASSERT(status, "failed to initialize GLAD");

        ENGINE_CORE_INFO("OpenGL info : ");
        ENGINE_CORE_INFO("\tVendor   : ", glGetString(GL_VENDOR));
        ENGINE_CORE_INFO("\tRenderer : ", glGetString(GL_RENDERER));
        ENGINE_CORE_INFO("\tVersion  : ", glGetString(GL_VERSION));
	}

	void GLFWOpenGLContext::swapBuffers() {
		ENGINE_PROFILE_FUNCTION();
        glfwSwapBuffers(window);
	}

	void GLFWOpenGLContext::enableVSync(bool enable){
		ENGINE_PROFILE_FUNCTION();
		if (enable){
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
	}

	// SDL2

	SDL2OpenGLContext::SDL2OpenGLContext(SDL_Window *window) : window{window}{}
	SDL2OpenGLContext::~SDL2OpenGLContext(){
		ENGINE_PROFILE_FUNCTION();
		SDL_GL_DeleteContext(context);
	}

	void SDL2OpenGLContext::init(){
		ENGINE_PROFILE_FUNCTION();
		context = SDL_GL_CreateContext(window);
		ENGINE_CORE_ASSERT(context, "SDL_GL_CreateContext : ", SDL_GetError());

		int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        ENGINE_CORE_ASSERT(status, "failed to initialize GLAD");

        ENGINE_CORE_INFO("OpenGL info : ");
        ENGINE_CORE_INFO("\tVendor   : ", glGetString(GL_VENDOR));
        ENGINE_CORE_INFO("\tRenderer : ", glGetString(GL_RENDERER));
        ENGINE_CORE_INFO("\tVersion  : ", glGetString(GL_VERSION));
	}

	void SDL2OpenGLContext::swapBuffers(){
		ENGINE_PROFILE_FUNCTION();
		SDL_GL_SwapWindow(window);
	}

	void SDL2OpenGLContext::enableVSync(bool enable){
		ENGINE_PROFILE_FUNCTION();
		if (enable){
			bool result = SDL_GL_SetSwapInterval(1) == 0;
			ENGINE_CORE_ASSERT(result, "unsuported swapInterval : ", SDL_GetError());
		} else {
			bool result = SDL_GL_SetSwapInterval(0) == 0;
			ENGINE_CORE_ASSERT(result, "unsuported swapInterval : ", SDL_GetError());
		}
	}
}