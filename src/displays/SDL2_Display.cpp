#include "engine/pch.hpp"
#include "engine/displays/SDL2_Display.hpp"
#include "engine/Exception.hpp"

#include "engine/events/WindowEvent.hpp"
#include "engine/events/Keycodes.hpp"
#include "engine/events/MouseEvent.hpp"
#include "engine/events/KeyEvent.hpp"

#include <libs/SDL2/SDL.h>

static uint8_t SDL2_windowCount = 0;

static void initializeSDL2(){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw engine::exceptions::API(std::string("SDL_Init : ") + SDL_GetError());
}

static int getWindowPosX(uint32_t pos, uint8_t monitorIndex){
	if (monitorIndex == 0) return pos;
	if (pos == engine::Display::POSITION_CENTERED) return SDL_WINDOWPOS_CENTERED;
	if (pos == engine::Display::POSITION_UNKNOWN) return SDL_WINDOWPOS_UNDEFINED;

	SDL_Rect bounds;
	SDL_GetDisplayBounds(monitorIndex, &bounds);
	return pos + bounds.x;
}

static int getWindowPosY(uint32_t pos, uint8_t monitorIndex){
	if (monitorIndex == 0) return pos;
	if (pos == engine::Display::POSITION_CENTERED) return SDL_WINDOWPOS_CENTERED;
	if (pos == engine::Display::POSITION_UNKNOWN) return SDL_WINDOWPOS_UNDEFINED;

	SDL_Rect bounds;
	SDL_GetDisplayBounds(monitorIndex, &bounds);
	return pos + bounds.y;
}

static SDL_Window* initializeNewWindow(const engine::Display::Definition &def){
	if (SDL2_windowCount == 0){
		initializeSDL2();
	}

	SDL2_windowCount++;
	uint32_t flags = 0;

	switch (def.renderAPI){
		case engine::RenderAPI::OPENGL: flags = SDL_WINDOW_OPENGL; break;
		case engine::RenderAPI::VULKAN: flags = SDL_WINDOW_VULKAN; break;
		// case engine::RenderAPI::METAL: flags = SDL_WINDOW_METAL; break;
	}

	SDL_Window *window = SDL_CreateWindow(def.title.c_str(), getWindowPosX(def.position.x, def.monitorIndex), getWindowPosY(def.position.y, def.monitorIndex), static_cast<int>(def.size.x), static_cast<int>(def.size.y), flags);

	if (window == nullptr) throw engine::exceptions::API(std::string("SDL_CreateWindow : ") + SDL_GetError());

	return window;
}

static SDL_Window* destroyWindow(SDL_Window* window){
	SDL_DestroyWindow(window);
	SDL2_windowCount--;
	return nullptr;
}

namespace engine::displays{
	
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

	Ref<SDL2_Display> SDL2_Display::create(const Definition &properties){
		Ref<SDL2_Display> display = createRef<SDL2_Display>(properties);
		SDL_SetWindowData(static_cast<SDL_Window*>(display->window), "data", display.get());

		return display;
	}

	SDL2_Display::SDL2_Display(const Definition &properties) : windowData{properties}{
		window = initializeNewWindow(properties);
	}

	SDL2_Display::~SDL2_Display(){
		window = destroyWindow(static_cast<SDL_Window*>(window));
	}

	glm::u32vec2 SDL2_Display::getSize() const{
		return windowData.size;
	}

	glm::u32vec2 SDL2_Display::getMinimalSize() const{
		return windowData.minimalSize;
	}

	glm::u32vec2 SDL2_Display::getMaximalSize() const{
		return windowData.maximalSize;
	}

	glm::u32vec2 SDL2_Display::getPosition() const{
		return windowData.position;
	}

	uint8_t SDL2_Display::getMonitorIndex() const{
		return windowData.monitorIndex;
	}

	float SDL2_Display::getOpacity() const{
		return windowData.opacity;
	}

	bool SDL2_Display::isFullscreen() const{
		return windowData.fullscreen;
	}

	bool SDL2_Display::isBordered() const{
		return windowData.bordered;
	}

	const std::string &SDL2_Display::getTitle() const{
		return windowData.title;
	}

	void SDL2_Display::resize(const glm::u32vec2 &size){
		SDL_SetWindowSize(static_cast<SDL_Window*>(window), static_cast<int>(size.x), static_cast<int>(size.y));
		// resize renderer
		windowData.size = size;
	}

	void SDL2_Display::setMinimalSize(const glm::u32vec2 &size){
		SDL_SetWindowMinimumSize(static_cast<SDL_Window*>(window), static_cast<int>(size.x), static_cast<int>(size.y));
		windowData.minimalSize = size;
	}

	void SDL2_Display::setMaximalSize(const glm::u32vec2 &size){
		SDL_SetWindowMaximumSize(static_cast<SDL_Window*>(window), static_cast<int>(size.x), static_cast<int>(size.y));
		windowData.maximalSize = size;
	}

	void SDL2_Display::move(const glm::u32vec2 &position){
		windowData.position = position;
		windowData.monitorIndex = static_cast<uint8_t>(SDL_GetWindowDisplayIndex(static_cast<SDL_Window*>(window)));
		glm::u32vec2 pos;
		pos.x = getWindowPosX(position.x, windowData.monitorIndex);
		pos.x = getWindowPosY(position.y, windowData.monitorIndex);
		windowData.monitorIndex = SDL_GetWindowDisplayIndex(static_cast<SDL_Window*>(window));
		SDL_SetWindowPosition(static_cast<SDL_Window*>(window), static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	void SDL2_Display::setMonitor(uint8_t monitorIndex){
		windowData.monitorIndex = monitorIndex;
		glm::u32vec2 pos;
		pos.x = getWindowPosX(windowData.position.x, monitorIndex);
		pos.x = getWindowPosY(windowData.position.y, monitorIndex);
		SDL_SetWindowPosition(static_cast<SDL_Window*>(window), static_cast<int>(pos.x), static_cast<int>(pos.y));
	}	

	void SDL2_Display::setTitle(const std::string &title){
		windowData.title = title;
		SDL_SetWindowTitle(static_cast<SDL_Window*>(window), title.c_str());
	}
	
	void* SDL2_Display::getNativeWindow() const{
		return window;
	}

	float SDL2_Display::getTime() const{
		return SDL_GetTicks64() / 1000.f;
	}

	float SDL2_Display::getAspectRatio() const{
		return static_cast<float>(windowData.size.x) / static_cast<float>(windowData.size.y);
	}

	void SDL2_Display::update(){
		// update renderer

		SDL_Event event;
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_WINDOWEVENT: onWindowEvent(&event); break;
				case SDL_KEYDOWN: onKeyDownEvent(&event); break;
				case SDL_KEYUP: onKeyUpEvent(&event); break;
				case SDL_MOUSEBUTTONDOWN: onMouseButtonDownEvent(&event); break;
				case SDL_MOUSEBUTTONUP: onMouseButtonUpEvent(&event); break;
				case SDL_MOUSEWHEEL: onMouseWheelEvent(&event); break;
				case SDL_MOUSEMOTION: onMouseMovedEvent(&event); break;
			}
		}
	}

	void SDL2_Display::onWindowEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		
		switch (e->window.event){
			case SDL_WINDOWEVENT_CLOSE: onWindowClosedEvent(event); break;
			case SDL_WINDOWEVENT_FOCUS_GAINED: onWindowFocusedEvent(event); break;
			case SDL_WINDOWEVENT_FOCUS_LOST: onWindowLostFocusEvent(event); break;
			case SDL_WINDOWEVENT_MOVED: onWindowMovedEvent(event); break;
			case SDL_WINDOWEVENT_MINIMIZED: onWindowMinimizedEvent(event); break;
			case SDL_WINDOWEVENT_MAXIMIZED: onWindowMaximizedEvent(event); break;
			case SDL_WINDOWEVENT_RESIZED: onWindowResizedEvent(event); break;
		}
	}

	void SDL2_Display::onWindowClosedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));
		
		WindowCloseEvent windowEvent{windowData.app, display};
		windowData.eventCallback(windowEvent);
	}

	void SDL2_Display::onWindowFocusedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));
		
		WindowFocusedEvent windowEvent{windowData.app, display};
		windowData.eventCallback(windowEvent);
	}

	void SDL2_Display::onWindowLostFocusEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		// when a window is quitted, it trigger the lost focus event event if the window is destroyed
		if (!window) return;
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));
		
		WindowLostFocusEvent windowEvent{windowData.app, display};
		windowData.eventCallback(windowEvent);
	}

	void SDL2_Display::onWindowMovedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));

		int x, y;
		SDL_GetWindowPosition(static_cast<SDL_Window*>(window), &x, &y);
		windowData.position.x = getWindowPosX(x, windowData.monitorIndex);
		windowData.position.x = getWindowPosY(y, windowData.monitorIndex);
		
		WindowMovedEvent windowEvent{windowData.app, display, windowData.position, windowData.monitorIndex};
		windowData.eventCallback(windowEvent);
	}
	
	void SDL2_Display::onWindowMinimizedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));

		WindowMaximizedEvent windowEvent{windowData.app, display};
		windowData.eventCallback(windowEvent);
	}

	void SDL2_Display::onWindowMaximizedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));
		
		WindowMaximizedEvent windowEvent{windowData.app, display};
		windowData.eventCallback(windowEvent);
	}

	void SDL2_Display::onWindowResizedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		SDL_Window* window = SDL_GetWindowFromID(e->window.windowID);
		ENGINE_CORE_ASSERT(window, "SDL_GetWindowFromID Error : ", SDL_GetError())
		SDL2_Display* display = reinterpret_cast<SDL2_Display*>(SDL_GetWindowData(window, "data"));
	
		resize({static_cast<uint32_t>(e->window.data1), static_cast<uint32_t>(e->window.data2)});
		WindowResizedEvent windowEvent{windowData.app, display, windowData.size.x, windowData.size.y};
		windowData.eventCallback(windowEvent);
	}
	
	void SDL2_Display::onKeyDownEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		KeyPressedEvent keyEvent{windowData.app, SDLToEngineKey(e->key.keysym.scancode), e->key.repeat};
		windowData.eventCallback(keyEvent);
	}

	void SDL2_Display::onKeyUpEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		KeyReleasedEvent keyEvent{windowData.app, SDLToEngineKey(e->key.keysym.scancode)};
		windowData.eventCallback(keyEvent);
	}
	
	void SDL2_Display::onMouseButtonDownEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		MouseButtonPressedEvent mouseEvent{windowData.app, e->button.button, e->button.clicks > 1};
		windowData.eventCallback(mouseEvent);
	}

	void SDL2_Display::onMouseButtonUpEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		MouseButtonReleasedEvent mouseEvent{windowData.app, e->button.button};
		windowData.eventCallback(mouseEvent);
	}

	void SDL2_Display::onMouseWheelEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		MouseScrolledEvent mouseEvent{windowData.app, e->wheel.preciseX, e->wheel.preciseY};
		windowData.eventCallback(mouseEvent);
	}

	void SDL2_Display::onMouseMovedEvent(void *event){
		SDL_Event *e = static_cast<SDL_Event*>(event);
		MouseMovedEvent mouseEvent{windowData.app, static_cast<float>(e->motion.x), static_cast<float>(e->motion.y)};
		windowData.eventCallback(mouseEvent);
	}

	void SDL2_Display::setEventCallback(const EventFn &callback){
		this->windowData.eventCallback = callback;
	}
}