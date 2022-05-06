#pragma once

#include "../core.hpp"
#include "Keycodes.hpp"
#include "KeyInput.hpp"

namespace engine{
	class ENGINE_API Input{
		public:
			virtual ~Input() {}

			virtual bool isKeyUp(Key keycode) const = 0;
			virtual bool isKeyDown(Key keycode) const = 0;
		
			virtual bool isMouseButtonDown(int button) const = 0;
			virtual bool isMouseButtonUp(int button) const = 0;
			
			virtual float getMouseX() const = 0;
			virtual float getMouseY() const = 0;

			virtual float getMouseWheel() const = 0;

			bool isMouseWheelUp() const {return getMouseWheel() > 0.f;}
			bool isMouseWheelDown() const {return getMouseWheel() < 0.f;}

			bool isDown(KeyInput &key, KeyPressedEvent e){
				return key.OnKeyPressed(e, this);
			}
	};
}