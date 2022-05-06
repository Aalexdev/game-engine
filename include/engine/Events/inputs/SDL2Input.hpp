#pragma once

#include "../../core.hpp"
#include "../Input.hpp"
#include "../Keycodes.hpp"

namespace engine::inputs{
	class SDL2Input : public Input{
		public:
			SDL2Input() {}
			~SDL2Input() {}

			virtual bool isKeyUp(Key keycode) const override {return !keyState[static_cast<int>(keycode)];}
			virtual bool isKeyDown(Key keycode) const override {return keyState[static_cast<int>(keycode)];}

			virtual bool isMouseButtonDown(int button) const override {return buttonState[button];}
			virtual bool isMouseButtonUp(int button) const override {return !buttonState[button];}

			virtual float getMouseX() const override {return mouseX;}
			virtual float getMouseY() const override {return mouseY;}
			
			virtual float getMouseWheel() const override {return mouseWheel;}

			bool* getKeyState() {return keyState;}
			bool* getButtonStats() {return buttonState;}
			void setMousePos(float x, float y) {mouseX = x; mouseY = y;}
			void setMouseWheel(float mouseWheel) {this->mouseWheel = mouseWheel;}
		
		private:
			bool keyState[static_cast<int>(Key::KEY_MAX)];
			bool buttonState[5];

			float mouseX = 0.f, mouseY = 0.f;
			float mouseWheel;
	};
}