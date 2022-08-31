#include "EventManager.hpp"
#include "Hermes.hpp"
#include "core.hpp"

#include <SDL2/SDL.h>
#include "RainDrop.hpp"
#include "Fovea.h"

namespace RainDrop{
	struct STDEvents{
		Hermes::EventID windowClosed;
		Hermes::EventID windowMoved;
		Hermes::EventID windowResized;
		Hermes::EventID windowFocusLost;
		Hermes::EventID windowFocusGained;
		Hermes::EventID windowMinimized;
		Hermes::EventID windowMaximized;
		Hermes::EventID windowRestored;

		Hermes::EventID mouseMoved;
		Hermes::EventID mouseButtonDown;
		Hermes::EventID mouseButtonUp;
		Hermes::EventID mouseScrolled;

		Hermes::EventID keyPressed;
		Hermes::EventID keyReleased;
	};

	STDEvents& getSTDEvents(){
		static STDEvents instance;
		return instance;
	}

	static MouseButton SDLMouseButtonToRainDropMouseButton(uint8_t button){
		switch (button){
			case SDL_BUTTON_LEFT: return MouseButton::Left;
			case SDL_BUTTON_RIGHT: return MouseButton::Right;
			case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
			case SDL_BUTTON_X1: return MouseButton::X1;
			case SDL_BUTTON_X2: return MouseButton::X2;
		}
		return MouseButton::Left;
	}

	static Key SDLToEngineKey(int key){
		switch (key){
			case SDLK_a: return Key::K_A;
			case SDLK_b: return Key::K_B;
			case SDLK_c: return Key::K_C;
			case SDLK_d: return Key::K_D;
			case SDLK_e: return Key::K_E;
			case SDLK_f: return Key::K_F;
			case SDLK_g: return Key::K_G;
			case SDLK_h: return Key::K_H;
			case SDLK_i: return Key::K_I;
			case SDLK_j: return Key::K_J;
			case SDLK_k: return Key::K_K;
			case SDLK_l: return Key::K_L;
			case SDLK_m: return Key::K_M;
			case SDLK_n: return Key::K_N;
			case SDLK_o: return Key::K_O;
			case SDLK_p: return Key::K_P;
			case SDLK_q: return Key::K_Q;
			case SDLK_r: return Key::K_R;
			case SDLK_s: return Key::K_S;
			case SDLK_t: return Key::K_T;
			case SDLK_u: return Key::K_U;
			case SDLK_v: return Key::K_V;
			case SDLK_w: return Key::K_W;
			case SDLK_x: return Key::K_X;
			case SDLK_y: return Key::K_Y;
			case SDLK_z: return Key::K_Z;
			case SDLK_1: return Key::K_1;
			case SDLK_2: return Key::K_2;
			case SDLK_3: return Key::K_3;
			case SDLK_4: return Key::K_4;
			case SDLK_5: return Key::K_5;
			case SDLK_6: return Key::K_6;
			case SDLK_7: return Key::K_7;
			case SDLK_8: return Key::K_8;
			case SDLK_9: return Key::K_9;
			case SDLK_0: return Key::K_0;
			case SDLK_RETURN: return Key::K_ENTER;
			case SDLK_ESCAPE: return Key::K_ESCAPE;
			case SDLK_BACKSPACE: return Key::K_BACKSPACE;
			case SDLK_TAB: return Key::K_TAB;
			case SDLK_SPACE: return Key::K_SPACE;
			case SDLK_MINUS: return Key::K_MINUS;
			case SDLK_EQUALS: return Key::K_EQUAL;
			case SDLK_LEFTBRACKET: return Key::K_LEFT_BRACKET;
			case SDLK_RIGHTBRACKET: return Key::K_RIGHT_BRACKET;
			case SDLK_BACKSLASH: return Key::K_BACKSLASH;
			case SDLK_SEMICOLON: return Key::K_SEMICOLON;
			case SDLK_COMMA: return Key::K_COMMA;
			case SDLK_PERIOD: return Key::K_PERIOD;
			case SDLK_SLASH: return Key::K_SLASH;
			case SDLK_CAPSLOCK: return Key::K_CAPS_LOCK;
			case SDLK_F1: return Key::K_F1;
			case SDLK_F2: return Key::K_F2;
			case SDLK_F3: return Key::K_F3;
			case SDLK_F4: return Key::K_F4;
			case SDLK_F5: return Key::K_F5;
			case SDLK_F6: return Key::K_F6;
			case SDLK_F7: return Key::K_F7;
			case SDLK_F8: return Key::K_F8;
			case SDLK_F9: return Key::K_F9;
			case SDLK_F10: return Key::K_F10;
			case SDLK_F11: return Key::K_F11;
			case SDLK_F12: return Key::K_F12;
			case SDLK_PRINTSCREEN: return Key::K_PRINT_SCREEN;
			case SDLK_SCROLLLOCK: return Key::K_SCROLL_LOCK;
			case SDLK_PAUSE: return Key::K_PAUSE;
			case SDLK_INSERT: return Key::K_INSERT;
			case SDLK_HOME: return Key::K_HOME;
			case SDLK_PAGEUP: return Key::K_PAGE_UP;
			case SDLK_DELETE: return Key::K_DELETE;
			case SDLK_END: return Key::K_END;
			case SDLK_PAGEDOWN: return Key::K_PAGE_DOWN;
			case SDLK_RIGHT: return Key::K_RIGHT;
			case SDLK_LEFT: return Key::K_LEFT;
			case SDLK_DOWN: return Key::K_DOWN;
			case SDLK_UP: return Key::K_UP;
			case SDLK_NUMLOCKCLEAR: return Key::K_NUM_LOCK;
			case SDLK_KP_DIVIDE: return Key::K_KP_DIVIDE;
			case SDLK_KP_MULTIPLY: return Key::K_KP_MULTIPLY;
			case SDLK_KP_MINUS: return Key::K_KP_SUBTRACT;
			case SDLK_KP_PLUS: return Key::K_KP_ADD;
			case SDLK_KP_ENTER: return Key::K_KP_ENTER;
			case SDLK_KP_1: return Key::K_KP_1;
			case SDLK_KP_2: return Key::K_KP_2;
			case SDLK_KP_3: return Key::K_KP_3;
			case SDLK_KP_4: return Key::K_KP_4;
			case SDLK_KP_5: return Key::K_KP_5;
			case SDLK_KP_6: return Key::K_KP_6;
			case SDLK_KP_7: return Key::K_KP_7;
			case SDLK_KP_8: return Key::K_KP_8;
			case SDLK_KP_9: return Key::K_KP_9;
			case SDLK_KP_0: return Key::K_KP_0;
			case SDLK_KP_DECIMAL: return Key::K_KP_DECIMAL;
			case SDLK_KP_EQUALS: return Key::K_KP_EQUAL;
			case SDLK_F13: return Key::K_F13;
			case SDLK_F14: return Key::K_F14;
			case SDLK_F15: return Key::K_F15;
			case SDLK_F16: return Key::K_F16;
			case SDLK_F17: return Key::K_F17;
			case SDLK_F18: return Key::K_F18;
			case SDLK_F19: return Key::K_F19;
			case SDLK_F20: return Key::K_F20;
			case SDLK_F21: return Key::K_F21;
			case SDLK_F22: return Key::K_F22;
			case SDLK_F23: return Key::K_F23;
			case SDLK_F24: return Key::K_F24;
			case SDLK_LCTRL: return Key::K_LEFT_CONTROL;
			case SDLK_LSHIFT: return Key::K_LEFT_SHIFT;
			case SDLK_LALT: return Key::K_LEFT_ALT;
			case SDLK_RCTRL: return Key::K_RIGHT_CONTROL;
			case SDLK_RSHIFT: return Key::K_RIGHT_SHIFT;
			case SDLK_RALT: return Key::K_RIGHT_ALT;
		}
		return Key::K_NONE;
	}
	
	const char* KeyToStr(Key key){
		switch (key){
			case Key::K_SPACE: return " ";
			case Key::K_COMMA: return ",";
			case Key::K_MINUS: return "-";
			case Key::K_PERIOD: return ".";
			case Key::K_SLASH: return "/";
			case Key::K_0: return "0";
			case Key::K_1: return "1";
			case Key::K_2: return "2";
			case Key::K_3: return "3";
			case Key::K_4: return "4";
			case Key::K_5: return "5";
			case Key::K_6: return "6";
			case Key::K_7: return "7";
			case Key::K_8: return "8";
			case Key::K_9: return "9";
			case Key::K_SEMICOLON: return ";";
			case Key::K_EQUAL: return "=";
			case Key::K_A: return "a";
			case Key::K_B: return "b";
			case Key::K_C: return "c";
			case Key::K_D: return "d";
			case Key::K_E: return "e";
			case Key::K_F: return "f";
			case Key::K_G: return "g";
			case Key::K_H: return "h";
			case Key::K_I: return "i";
			case Key::K_J: return "j";
			case Key::K_K: return "k";
			case Key::K_L: return "l";
			case Key::K_M: return "m";
			case Key::K_N: return "n";
			case Key::K_O: return "o";
			case Key::K_P: return "p";
			case Key::K_Q: return "q";
			case Key::K_R: return "r";
			case Key::K_S: return "s";
			case Key::K_T: return "t";
			case Key::K_U: return "u";
			case Key::K_V: return "v";
			case Key::K_W: return "w";
			case Key::K_X: return "x";
			case Key::K_Y: return "y";
			case Key::K_Z: return "z";
			case Key::K_LEFT_BRACKET: return "[";
			case Key::K_BACKSLASH: return "\\";
			case Key::K_RIGHT_BRACKET: return "]";
			case Key::K_ESCAPE: return "Escape";
			case Key::K_ENTER: return "Enter";
			case Key::K_TAB: return "Tab";
			case Key::K_BACKSPACE: return "Backspace";
			case Key::K_INSERT: return "Insert";
			case Key::K_DELETE: return "Delete";
			case Key::K_RIGHT: return "Right";
			case Key::K_LEFT: return "Left";
			case Key::K_UP: return "Up";
			case Key::K_DOWN: return "Down";
			case Key::K_PAGE_UP: return "Page Up";
			case Key::K_PAGE_DOWN: return "Page Down";
			case Key::K_HOME: return "Home";
			case Key::K_END: return "End";
			case Key::K_CAPS_LOCK: return "Caps Lock";
			case Key::K_SCROLL_LOCK: return "Scroll Lock";
			case Key::K_NUM_LOCK: return "Num Lock";
			case Key::K_PRINT_SCREEN: return "Print Screen";
			case Key::K_PAUSE: return "Pause";
			case Key::K_F1: return "F1";
			case Key::K_F2: return "F2";
			case Key::K_F3: return "F3";
			case Key::K_F4: return "F4";
			case Key::K_F5: return "F5";
			case Key::K_F6: return "F6";
			case Key::K_F7: return "F7";
			case Key::K_F8: return "F8";
			case Key::K_F9: return "F9";
			case Key::K_F10: return "F10";
			case Key::K_F11: return "F11";
			case Key::K_F12: return "F12";
			case Key::K_F13: return "F13";
			case Key::K_F14: return "F14";
			case Key::K_F15: return "F15";
			case Key::K_F16: return "F16";
			case Key::K_F17: return "F17";
			case Key::K_F18: return "F18";
			case Key::K_F19: return "F19";
			case Key::K_F20: return "F20";
			case Key::K_F21: return "F21";
			case Key::K_F22: return "F22";
			case Key::K_F23: return "F23";
			case Key::K_F24: return "F24";
			case Key::K_KP_0: return "Keypad 0";
			case Key::K_KP_1: return "Keypad 1";
			case Key::K_KP_2: return "Keypad 2";
			case Key::K_KP_3: return "Keypad 3";
			case Key::K_KP_4: return "Keypad 4";
			case Key::K_KP_5: return "Keypad 5";
			case Key::K_KP_6: return "Keypad 6";
			case Key::K_KP_7: return "Keypad 7";
			case Key::K_KP_8: return "Keypad 8";
			case Key::K_KP_9: return "Keypad 9";
			case Key::K_KP_DECIMAL: return "Keypad Decimal";
			case Key::K_KP_DIVIDE: return "Keypad Divide";
			case Key::K_KP_MULTIPLY: return "Keypad Multiply";
			case Key::K_KP_SUBTRACT: return "Keypad Subtract";
			case Key::K_KP_ADD: return "Keypad Add";
			case Key::K_KP_ENTER: return "Keypad Enter";
			case Key::K_KP_EQUAL: return "Keypad Equal";
			case Key::K_LEFT_SHIFT: return "Left Shift";
			case Key::K_RIGHT_SHIFT: return "Right Shift";
			case Key::K_LEFT_ALT: return "Left Alt";
			case Key::K_RIGHT_ALT: return "Right Alt";
			case Key::K_LEFT_CONTROL: return "Left Ctr";
			case Key::K_RIGHT_CONTROL: return "Right Ctr";
			case Key::K_MENU: return "Menu";
		}
		return "";
	}

	void registerEvents(){
		STDEvents& instance = getSTDEvents();
		Hermes::DataLayout layout;

		instance.windowClosed = Hermes::registerEvent("window closed");
		instance.windowFocusLost = Hermes::registerEvent("window focus lost");
		instance.windowFocusGained = Hermes::registerEvent("window focus gained");
		instance.windowMinimized = Hermes::registerEvent("window minimized");
		instance.windowMaximized = Hermes::registerEvent("window maximized");
		instance.windowRestored = Hermes::registerEvent("window restored");
		instance.windowMoved = Hermes::registerEvent("window moved", Hermes::DataLayout::construct<vec2<uint32_t>>());
		instance.windowResized = Hermes::registerEvent("window resized", Hermes::DataLayout::construct<vec2<uint32_t>>());

		instance.mouseMoved = Hermes::registerEvent("mouse moved", Hermes::DataLayout::construct<vec2<float>>());
		instance.mouseButtonDown = Hermes::registerEvent("mouse buttton down", Hermes::DataLayout::construct<MouseButton>());
		instance.mouseButtonUp = Hermes::registerEvent("mouse button up", Hermes::DataLayout::construct<MouseButton>());
		instance.mouseScrolled = Hermes::registerEvent("mouse scroled", Hermes::DataLayout::construct<vec2<float>>());

		instance.keyPressed = Hermes::registerEvent("key pressed", Hermes::DataLayout::construct<Key, bool>());
		instance.keyReleased = Hermes::registerEvent("key released", Hermes::DataLayout::construct<Key, bool>());
	}

	void poolEvents(){
		STDEvents& instance = getSTDEvents();
		
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_QUIT:{
					Hermes::triggerEvent(instance.windowClosed); 
					break;
				}
				
				case SDL_WINDOWEVENT:{
					switch (e.window.event){
						case SDL_WINDOWEVENT_MOVED:{
							Hermes::triggerEvent(instance.windowMoved, vec2<uint32_t>{static_cast<uint32_t>(e.window.data1), static_cast<uint32_t>(e.window.data2)});
							break;
						}

						case SDL_WINDOWEVENT_RESIZED:{
							Hermes::triggerEvent(instance.windowResized, vec2<uint32_t>{static_cast<uint32_t>(e.window.data1), static_cast<uint32_t>(e.window.data2)});
							FoveaOnWindowResized(static_cast<uint32_t>(e.window.data1), static_cast<uint32_t>(e.window.data2));
							break;
						}

						case SDL_WINDOWEVENT_FOCUS_LOST:{
							Hermes::triggerEvent(instance.windowFocusLost);
							break;
						}

						case SDL_WINDOWEVENT_FOCUS_GAINED:{
							Hermes::triggerEvent(instance.windowFocusGained);
							break;
						}

						case SDL_WINDOWEVENT_MINIMIZED:{
							Hermes::triggerEvent(instance.windowMinimized);
							break;
						}

						case SDL_WINDOWEVENT_MAXIMIZED:{
							Hermes::triggerEvent(instance.windowMaximized);
							break;
						}
					}
					break;
				}
				
				case SDL_MOUSEBUTTONDOWN:{
					getInstance().buttonPressed[static_cast<int>(SDLMouseButtonToRainDropMouseButton(e.button.button))] = true;
					Hermes::triggerEvent(instance.mouseButtonDown, SDLMouseButtonToRainDropMouseButton(e.button.button));
					break;
				}

				case SDL_MOUSEBUTTONUP:{
					getInstance().buttonPressed[static_cast<int>(SDLMouseButtonToRainDropMouseButton(e.button.button))] = false;
					Hermes::triggerEvent(instance.mouseButtonUp, SDLMouseButtonToRainDropMouseButton(e.button.button));
					break;
				}

				case SDL_MOUSEMOTION:{
					getInstance().mousePos = vec2<float>{static_cast<float>(e.motion.x), static_cast<float>(e.motion.y)};
					Hermes::triggerEvent(instance.mouseMoved, getInstance().mousePos);
					break;
				}

				case SDL_MOUSEWHEEL:{
					Hermes::triggerEvent(instance.mouseScrolled, vec2<float>{e.wheel.preciseX, e.wheel.preciseY});
					break;
				}

				case SDL_KEYDOWN:{
					getInstance().keyPressed[static_cast<int>(SDLToEngineKey(e.key.keysym.sym))] = true;
					Hermes::triggerEvent(instance.keyPressed, SDLToEngineKey(e.key.keysym.sym), static_cast<bool>(e.key.repeat));
					break;
				}

				case SDL_KEYUP:{
					getInstance().keyPressed[static_cast<int>(SDLToEngineKey(e.key.keysym.sym))] = false;
					Hermes::triggerEvent(instance.keyReleased, SDLToEngineKey(e.key.keysym.sym), static_cast<bool>(e.key.repeat));
					break;
				}

			}
		}
	}

}