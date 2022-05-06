#include "engine/pch.hpp"
#include "engine/windows/Window.hpp"

#include "engine/windows/GLFWWindow.hpp"
#include "engine/windows/SDL2Window.hpp"

namespace engine{
	Window::WindowAPI Window::windowAPI = Window::WindowAPI::GLFW;

	Ref<Window> Window::create(const Properties &properties){
		switch (windowAPI){
			case WindowAPI::GLFW: return createRef<windows::GLFWWindow>(properties);
			case WindowAPI::SDL2: return createRef<windows::SDL2Window>(properties);
		}
		return nullptr;
	}
}