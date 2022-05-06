#include "engine/pch.hpp"
#include "engine/windows/GLFWWindow.hpp"
#include "engine/debug/Log.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/Events/ApplicationEvent.hpp"
#include "engine/Events/KeyEvent.hpp"
#include "engine/Events/MouseEvent.hpp"
#include "engine/Events/MouseButtons.hpp"

#include <libs/GLFW/glfw3.h>

namespace engine::windows{
	static uint8_t glfwWindowCount = 0;

	static void GLFWErrorCallback(int error, const char *description){
        ENGINE_CORE_ERROR("GLFW error : ", error, " | ", description);
    }

	GLFWWindow::GLFWWindow(const Properties &properties){
		ENGINE_PROFILE_FUNCTION();
		initGLFW();
		initWindow(properties);

		data.inputs = createRef<inputs::GLFWInput>();
		(*data.inputs->getWindow()) = window;
	}

	GLFWWindow::~GLFWWindow(){
		ENGINE_PROFILE_FUNCTION();
		shutdown();
		shutdownGLFW();
	}

	uint32_t GLFWWindow::getWidth() const{
		return data.width;
	}

	uint32_t GLFWWindow::getHeight() const{
		return data.height;
	}

	void GLFWWindow::setVSync(bool enable){
		data.VSync = enable;
		context->enableVSync(enable);
	}

	bool GLFWWindow::isVSyncEnable() const{
		return data.VSync;
	}

	void* GLFWWindow::getNativeWindow() const{
		return window;
	}

	float GLFWWindow::getTime() const{
		return glfwGetTime();
	}

	float GLFWWindow::getAspectRatio() const{
		return static_cast<float>(data.width) / static_cast<float>(data.height);
	}

	void GLFWWindow::initWindow(const Properties &properties){
		ENGINE_PROFILE_FUNCTION();
		data.width = properties.width;
        data.height = properties.height;
        data.title = properties.title;

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		
		{
			ENGINE_PROFILE_SCOPE("create GLFW window");
			ENGINE_CORE_INFO("create GLFW window");
			window = glfwCreateWindow(static_cast<int>(properties.width), static_cast<int>(properties.height), properties.title.c_str(), nullptr, nullptr);
		}

		context = Context::create(this);
		context->init();

		glfwSetWindowUserPointer(window, &data);

		ENGINE_ASSERT(window, "glfwCreateWindow : failed to create a GLFW window");
		glfwWindowCount++;

		{
			ENGINE_PROFILE_SCOPE("setup callbacks");
			glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowResizedEvent event(width, height);
				data.width = width;
				data.height = height;

				data.EventCallback(event);
			});

			glfwSetWindowCloseCallback(window, [](GLFWwindow* window){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				WindowCloseEvent event;
				data.EventCallback(event);
			});

			glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				
				switch (action){
					case GLFW_RELEASE:{
						KeyReleasedEvent event(static_cast<Key>(key));
						data.EventCallback(event);
						break;
					}

					case GLFW_PRESS:{
						KeyPressedEvent event(static_cast<Key>(key), 0);
						data.EventCallback(event);
						break;
					}
					
					case GLFW_REPEAT:{
						KeyPressedEvent event(static_cast<Key>(key), 1);
						data.EventCallback(event);
						break;
					}
				}
			});

			glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				KeyTypedEvent event(reinterpret_cast<char*>(&character));
				data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

				static float timeLastClick[ENGINE_MOUSE_BUTTON_LAST+1];

				switch (action){
					case GLFW_PRESS:{
						float timeSinceLastClick = glfwGetTime() - timeLastClick[button];
						timeLastClick[button] = static_cast<float>(glfwGetTime());

						bool doubleClicked = timeSinceLastClick <= 0.2;
						MouseButtonPressedEvent event(button, doubleClicked);
						data.EventCallback(event);
						break;
					}
					
					case GLFW_RELEASE:{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseMovedEvent event(static_cast<float>(x), static_cast<float>(y));
				data.EventCallback(event);
			});

			glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset){
				WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
				MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
				data.EventCallback(event);
				data.inputs->setMouseWheel(yOffset);
			});

			glfwSetErrorCallback(GLFWErrorCallback);
		}
	}

	void GLFWWindow::shutdown(){
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_INFO("destroy GLFW window");
		glfwDestroyWindow(window);
		glfwWindowCount--;
	}

	void GLFWWindow::initGLFW(){
		ENGINE_PROFILE_FUNCTION();
		if (glfwWindowCount == 0){
			ENGINE_PROFILE_SCOPE("init GLFW");
			ENGINE_CORE_INFO("initialize GLFW");
			bool result = glfwInit() == GLFW_TRUE;
			ENGINE_CORE_ASSERT(result, "glfwInit : failed to intialize GLFW")	
		}
		
	}

	void GLFWWindow::shutdownGLFW(){
		ENGINE_PROFILE_FUNCTION();
		if (glfwWindowCount == 0){
			ENGINE_PROFILE_SCOPE("terminate GLFW");
			ENGINE_CORE_INFO("terminate GLFW");
			glfwTerminate();
		}
	}

	void GLFWWindow::setEventCallback(const EventCallbackFn& callback){
		data.EventCallback = callback;
	}

	void GLFWWindow::update(){
		ENGINE_PROFILE_FUNCTION();
		glfwPollEvents();
		context->swapBuffers();
	}

	void GLFWWindow::setTitle(const std::string &title){
		glfwSetWindowTitle(window, title.c_str());
	}
}