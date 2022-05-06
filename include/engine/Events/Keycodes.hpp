#pragma once

#include "../core.hpp"
#include "../debug/Log.hpp"

// kecodes from GLFW3
namespace engine{
	enum class Key{
		KEY_NONE = 0,
		KEY_SPACE = 32,
		KEY_APOSTROPHE = 39,
		KEY_COMMA = 44,
		KEY_MINUS = 45,
		KEY_PERIOD = 46,
		KEY_SLASH = 47,
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		KEY_SEMICOLON = 59,
		KEY_EQUAL = 61,
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
		KEY_LEFT_BRACKET = 91,
		KEY_BACKSLASH = 92,
		KEY_RIGHT_BRACKET = 93,
		KEY_GRAVE_ACCENT = 96,
		KEY_WORLD_1 = 161,
		KEY_WORLD_2 = 162,
		KEY_ESCAPE = 256,
		KEY_ENTER = 257,
		KEY_TAB = 258,
		KEY_BACKSPACE = 259,
		KEY_INSERT = 260,
		KEY_DELETE = 261,
		KEY_RIGHT = 262,
		KEY_LEFT = 263,
		KEY_DOWN = 264,
		KEY_UP = 265,
		KEY_PAGE_UP = 266,
		KEY_PAGE_DOWN = 267,
		KEY_HOME = 268,
		KEY_END = 269,
		KEY_CAPS_LOCK = 280,
		KEY_SCROLL_LOCK = 281,
		KEY_NUM_LOCK = 282,
		KEY_PRINT_SCREEN = 283,
		KEY_PAUSE = 284,
		KEY_F1 = 290,
		KEY_F2 = 291,
		KEY_F3 = 292,
		KEY_F4 = 293,
		KEY_F5 = 294,
		KEY_F6 = 295,
		KEY_F7 = 296,
		KEY_F8 = 297,
		KEY_F9 = 298,
		KEY_F10 = 299,
		KEY_F11 = 300,
		KEY_F12 = 301,
		KEY_F13 = 302,
		KEY_F14 = 303,
		KEY_F15 = 304,
		KEY_F16 = 305,
		KEY_F17 = 306,
		KEY_F18 = 307,
		KEY_F19 = 308,
		KEY_F20 = 309,
		KEY_F21 = 310,
		KEY_F22 = 311,
		KEY_F23 = 312,
		KEY_F24 = 313,
		KEY_F25 = 314,
		KEY_KP_0 = 320,
		KEY_KP_1 = 321,
		KEY_KP_2 = 322,
		KEY_KP_3 = 323,
		KEY_KP_4 = 324,
		KEY_KP_5 = 325,
		KEY_KP_6 = 326,
		KEY_KP_7 = 327,
		KEY_KP_8 = 328,
		KEY_KP_9 = 329,
		KEY_KP_DECIMAL = 330,
		KEY_KP_DIVIDE = 331,
		KEY_KP_MULTIPLY = 332,
		KEY_KP_SUBTRACT = 333,
		KEY_KP_ADD = 334,
		KEY_KP_ENTER = 335,
		KEY_KP_EQUAL = 336,
		KEY_LEFT_SHIFT = 340,
		KEY_LEFT_CONTROL = 341,
		KEY_LEFT_ALT = 342,
		KEY_LEFT_SUPER = 343,
		KEY_RIGHT_SHIFT = 344,
		KEY_RIGHT_CONTROL = 345,
		KEY_RIGHT_ALT = 346,
		KEY_RIGHT_SUPER = 347,
		KEY_MENU = 348,
		KEY_MAX = 348,
	};


	// values from SDL_scancode.h
	static constexpr Key SDLToEngineKey(int key){
		switch (key){
			case 4: return Key::KEY_A;
			case 5: return Key::KEY_B;
			case 6: return Key::KEY_C;
			case 7: return Key::KEY_D;
			case 8: return Key::KEY_E;
			case 9: return Key::KEY_F;
			case 10: return Key::KEY_G;
			case 11: return Key::KEY_H;
			case 12: return Key::KEY_I;
			case 13: return Key::KEY_J;
			case 14: return Key::KEY_K;
			case 15: return Key::KEY_L;
			case 16: return Key::KEY_M;
			case 17: return Key::KEY_N;
			case 18: return Key::KEY_O;
			case 19: return Key::KEY_P;
			case 20: return Key::KEY_Q;
			case 21: return Key::KEY_R;
			case 22: return Key::KEY_S;
			case 23: return Key::KEY_T;
			case 24: return Key::KEY_U;
			case 25: return Key::KEY_V;
			case 26: return Key::KEY_W;
			case 27: return Key::KEY_X;
			case 28: return Key::KEY_Y;
			case 29: return Key::KEY_Z;
			case 30: return Key::KEY_1;
			case 31: return Key::KEY_2;
			case 32: return Key::KEY_3;
			case 33: return Key::KEY_4;
			case 34: return Key::KEY_5;
			case 35: return Key::KEY_6;
			case 36: return Key::KEY_7;
			case 37: return Key::KEY_8;
			case 38: return Key::KEY_9;
			case 39: return Key::KEY_0;
			case 40: return Key::KEY_ENTER;
			case 41: return Key::KEY_ESCAPE;
			case 42: return Key::KEY_BACKSPACE;
			case 43: return Key::KEY_TAB;
			case 44: return Key::KEY_SPACE;
			case 45: return Key::KEY_MINUS;
			case 46: return Key::KEY_EQUAL;
			case 47: return Key::KEY_LEFT_BRACKET;
			case 48: return Key::KEY_RIGHT_BRACKET;
			case 49: return Key::KEY_BACKSLASH;
			case 51: return Key::KEY_SEMICOLON;
			case 52: return Key::KEY_APOSTROPHE;
			case 53: return Key::KEY_GRAVE_ACCENT;
			case 54: return Key::KEY_COMMA;
			case 55: return Key::KEY_PERIOD;
			case 56: return Key::KEY_SLASH;
			case 57: return Key::KEY_CAPS_LOCK;
			case 58: return Key::KEY_F1;
			case 59: return Key::KEY_F2;
			case 60: return Key::KEY_F3;
			case 61: return Key::KEY_F4;
			case 62: return Key::KEY_F5;
			case 63: return Key::KEY_F6;
			case 64: return Key::KEY_F7;
			case 65: return Key::KEY_F8;
			case 66: return Key::KEY_F9;
			case 67: return Key::KEY_F10;
			case 68: return Key::KEY_F11;
			case 69: return Key::KEY_F12;
			case 70: return Key::KEY_PRINT_SCREEN;
			case 71: return Key::KEY_SCROLL_LOCK;
			case 72: return Key::KEY_PAUSE;
			case 73: return Key::KEY_INSERT;
			case 74: return Key::KEY_HOME;
			case 75: return Key::KEY_PAGE_UP;
			case 76: return Key::KEY_DELETE;
			case 77: return Key::KEY_END;
			case 78: return Key::KEY_PAGE_DOWN;
			case 79: return Key::KEY_RIGHT;
			case 80: return Key::KEY_LEFT;
			case 81: return Key::KEY_DOWN;
			case 82: return Key::KEY_UP;
			case 83: return Key::KEY_NUM_LOCK;
			case 84: return Key::KEY_KP_DIVIDE;
			case 85: return Key::KEY_KP_MULTIPLY;
			case 86: return Key::KEY_KP_SUBTRACT;
			case 87: return Key::KEY_KP_ADD;
			case 88: return Key::KEY_KP_ENTER;
			case 89: return Key::KEY_KP_1;
			case 90: return Key::KEY_KP_2;
			case 91: return Key::KEY_KP_3;
			case 92: return Key::KEY_KP_4;
			case 93: return Key::KEY_KP_5;
			case 94: return Key::KEY_KP_6;
			case 95: return Key::KEY_KP_7;
			case 96: return Key::KEY_KP_8;
			case 97: return Key::KEY_KP_9;
			case 98: return Key::KEY_KP_0;
			case 99: return Key::KEY_KP_DECIMAL;
			case 101: return Key::KEY_HOME;
			case 103: return Key::KEY_KP_EQUAL;
			case 104: return Key::KEY_F13;
			case 105: return Key::KEY_F14;
			case 106: return Key::KEY_F15;
			case 107: return Key::KEY_F16;
			case 108: return Key::KEY_F17;
			case 109: return Key::KEY_F18;
			case 110: return Key::KEY_F19;
			case 111: return Key::KEY_F20;
			case 112: return Key::KEY_F21;
			case 113: return Key::KEY_F22;
			case 114: return Key::KEY_F23;
			case 115: return Key::KEY_F25;
			case 224: return Key::KEY_LEFT_CONTROL;
			case 225: return Key::KEY_LEFT_SHIFT;
			case 226: return Key::KEY_LEFT_ALT;
			case 227: return Key::KEY_LEFT_SUPER;
			case 228: return Key::KEY_RIGHT_CONTROL;
			case 229: return Key::KEY_RIGHT_SHIFT;
			case 230: return Key::KEY_RIGHT_ALT;
			case 231: return Key::KEY_RIGHT_SUPER;
		}
		ENGINE_CORE_ASSERT(false, "unknown key");
		return Key::KEY_NONE;
	}
	
	// convert the given key into a const char array, return a blank string if unknown key
	static constexpr const char* KeyToStr(Key key){
		switch (key){
			case Key::KEY_SPACE: return " ";
			case Key::KEY_APOSTROPHE: return "'";
			case Key::KEY_COMMA: return ",";
			case Key::KEY_MINUS: return "-";
			case Key::KEY_PERIOD: return ".";
			case Key::KEY_SLASH: return "/";
			case Key::KEY_0: return "0";
			case Key::KEY_1: return "1";
			case Key::KEY_2: return "2";
			case Key::KEY_3: return "3";
			case Key::KEY_4: return "4";
			case Key::KEY_5: return "5";
			case Key::KEY_6: return "6";
			case Key::KEY_7: return "7";
			case Key::KEY_8: return "8";
			case Key::KEY_9: return "9";
			case Key::KEY_SEMICOLON: return ";";
			case Key::KEY_EQUAL: return "=";
			case Key::KEY_A: return "a";
			case Key::KEY_B: return "b";
			case Key::KEY_C: return "c";
			case Key::KEY_D: return "d";
			case Key::KEY_E: return "e";
			case Key::KEY_F: return "f";
			case Key::KEY_G: return "g";
			case Key::KEY_H: return "h";
			case Key::KEY_I: return "i";
			case Key::KEY_J: return "j";
			case Key::KEY_K: return "k";
			case Key::KEY_L: return "l";
			case Key::KEY_M: return "m";
			case Key::KEY_N: return "n";
			case Key::KEY_O: return "o";
			case Key::KEY_P: return "p";
			case Key::KEY_Q: return "q";
			case Key::KEY_R: return "r";
			case Key::KEY_S: return "s";
			case Key::KEY_T: return "t";
			case Key::KEY_U: return "u";
			case Key::KEY_V: return "v";
			case Key::KEY_W: return "w";
			case Key::KEY_X: return "x";
			case Key::KEY_Y: return "y";
			case Key::KEY_Z: return "z";
			case Key::KEY_LEFT_BRACKET: return "[";
			case Key::KEY_BACKSLASH: return "\\";
			case Key::KEY_RIGHT_BRACKET: return "]";
			case Key::KEY_GRAVE_ACCENT: return "`";
			case Key::KEY_ESCAPE: return "Escape";
			case Key::KEY_ENTER: return "Enter";
			case Key::KEY_TAB: return "Tab";
			case Key::KEY_BACKSPACE: return "Backspace";
			case Key::KEY_INSERT: return "Insert";
			case Key::KEY_DELETE: return "Delete";
			case Key::KEY_RIGHT: return "Right";
			case Key::KEY_LEFT: return "Left";
			case Key::KEY_UP: return "Up";
			case Key::KEY_DOWN: return "Down";
			case Key::KEY_PAGE_UP: return "Page Up";
			case Key::KEY_PAGE_DOWN: return "Page Down";
			case Key::KEY_HOME: return "Home";
			case Key::KEY_END: return "End";
			case Key::KEY_CAPS_LOCK: return "Caps Lock";
			case Key::KEY_SCROLL_LOCK: return "Scroll Lock";
			case Key::KEY_NUM_LOCK: return "Num Lock";
			case Key::KEY_PRINT_SCREEN: return "Print Screen";
			case Key::KEY_PAUSE: return "Pause";
			case Key::KEY_F1: return "F1";
			case Key::KEY_F2: return "F2";
			case Key::KEY_F3: return "F3";
			case Key::KEY_F4: return "F4";
			case Key::KEY_F5: return "F5";
			case Key::KEY_F6: return "F6";
			case Key::KEY_F7: return "F7";
			case Key::KEY_F8: return "F8";
			case Key::KEY_F9: return "F9";
			case Key::KEY_F10: return "F10";
			case Key::KEY_F11: return "F11";
			case Key::KEY_F12: return "F12";
			case Key::KEY_F13: return "F13";
			case Key::KEY_F14: return "F14";
			case Key::KEY_F15: return "F15";
			case Key::KEY_F16: return "F16";
			case Key::KEY_F17: return "F17";
			case Key::KEY_F18: return "F18";
			case Key::KEY_F19: return "F19";
			case Key::KEY_F20: return "F20";
			case Key::KEY_F21: return "F21";
			case Key::KEY_F22: return "F22";
			case Key::KEY_F23: return "F23";
			case Key::KEY_F24: return "F24";
			case Key::KEY_F25: return "F25";
			case Key::KEY_KP_0: return "Keypad 0";
			case Key::KEY_KP_1: return "Keypad 1";
			case Key::KEY_KP_2: return "Keypad 2";
			case Key::KEY_KP_3: return "Keypad 3";
			case Key::KEY_KP_4: return "Keypad 4";
			case Key::KEY_KP_5: return "Keypad 5";
			case Key::KEY_KP_6: return "Keypad 6";
			case Key::KEY_KP_7: return "Keypad 7";
			case Key::KEY_KP_8: return "Keypad 8";
			case Key::KEY_KP_9: return "Keypad 9";
			case Key::KEY_KP_DECIMAL: return "Keypad Decimal";
			case Key::KEY_KP_DIVIDE: return "Keypad Divide";
			case Key::KEY_KP_MULTIPLY: return "Keypad Multiply";
			case Key::KEY_KP_SUBTRACT: return "Keypad Subtract";
			case Key::KEY_KP_ADD: return "Keypad Add";
			case Key::KEY_KP_ENTER: return "Keypad Enter";
			case Key::KEY_KP_EQUAL: return "Keypad Equal";
			case Key::KEY_LEFT_SHIFT: return "Left Shift";
			case Key::KEY_RIGHT_SHIFT: return "Right Shift";
			case Key::KEY_LEFT_ALT: return "Left Alt";
			case Key::KEY_RIGHT_ALT: return "Right Alt";
			case Key::KEY_LEFT_CONTROL: return "Left Ctr";
			case Key::KEY_RIGHT_CONTROL: return "Right Ctr";
			case Key::KEY_LEFT_SUPER: return "Left Super";
			case Key::KEY_RIGHT_SUPER: return "Right Super";
			case Key::KEY_MENU: return "Menu";
		}
		return "";
	}
	
	inline std::ostream& operator<<(std::ostream& os, const Key &e){
        return os << KeyToStr(e);
    }

}