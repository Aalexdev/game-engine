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