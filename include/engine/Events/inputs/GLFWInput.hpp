#pragma once

#include "../../core.hpp"
#include "../Input.hpp"

#include <libs/GLFW/glfw3.h>

namespace engine::inputs{
	class ENGINE_API GLFWInput : public Input{
		public:
			GLFWInput() {}
			~GLFWInput() {}

			virtual bool isKeyUp(Key keycode) const override {return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_RELEASE;}
			virtual bool isKeyDown(Key keycode) const override {return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_PRESS;}
			virtual bool isMouseButtonDown(int button) const override {return glfwGetMouseButton(window, button) == GLFW_PRESS;}
			virtual bool isMouseButtonUp(int button) const override {return glfwGetMouseButton(window, button) == GLFW_RELEASE;}

			virtual float getMouseX() const override{
				double x;
				glfwGetCursorPos(window, &x, nullptr);
				return x;
			}

			virtual float getMouseY() const override{
				double y;
				glfwGetCursorPos(window, nullptr, &y);
				return y;
			}

			virtual float getMouseWheel() const override{
				return mouseWheel;
			}

			inline void setMouseWheel(float mouseWheel) {this->mouseWheel = mouseWheel;}
			GLFWwindow **getWindow() {return &window;}
		
		private:
			GLFWwindow *window;
			float mouseWheel = 0.f;
	};
}