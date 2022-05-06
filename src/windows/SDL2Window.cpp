#include "engine/pch.hpp"
#include "engine/windows/SDL2Window.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/Events/ApplicationEvent.hpp"
#include "engine/Events/KeyEvent.hpp"
#include "engine/Events/MouseEvent.hpp"
#include "engine/Events/MouseButtons.hpp"

#include <libs/SDL2/SDL.h>

namespace engine::windows{

	static uint8_t SDL2WindowCount = 0;

	SDL2Window::SDL2Window(const Properties &properties){
		ENGINE_PROFILE_FUNCTION();
		initSDL2();
		initWindow(properties);

		inputs = createRef<inputs::SDL2Input>();
	}

	SDL2Window::~SDL2Window(){
		ENGINE_PROFILE_FUNCTION();
		shutdown();
		shutdownSDL2();
	}

	uint32_t SDL2Window::getWidth() const{
		return data.width;
	}

	uint32_t SDL2Window::getHeight() const{
		return data.height;
	}

	void SDL2Window::setVSync(bool enable){
		ENGINE_PROFILE_FUNCTION();
		data.VSync = enable;
		context->enableVSync(enable);
	}

	bool SDL2Window::isVSyncEnable() const{
		return data.VSync;
	}

	void* SDL2Window::getNativeWindow() const{
		return window;
	}

	float SDL2Window::getTime() const{
		return static_cast<float>(SDL_GetTicks64()) / 1000.f;
	}

	float SDL2Window::getAspectRatio() const{
		return static_cast<float>(data.width) / static_cast<float>(data.height);
	}

	void SDL2Window::initWindow(const Properties &properties){
		ENGINE_PROFILE_FUNCTION();
		data.width = properties.width;
		data.height = properties.height;
		data.title = properties.title;

		uint32_t windowFlags = 0;

		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: windowFlags = SDL_WINDOW_OPENGL; break;
			case Renderer::RenderAPI::Vulkan: windowFlags = SDL_WINDOW_VULKAN; break;
			case Renderer::RenderAPI::Metal:  windowFlags = SDL_WINDOW_METAL;  break;
			case Renderer::RenderAPI::DirectX: ENGINE_CORE_ASSERT(0, "DirectX is not supported by SDL2"); break;
		}

		ENGINE_CORE_ASSERT(windowFlags != 0, "unknown renderAPI")

		{
			ENGINE_PROFILE_SCOPE("create SDL2 window");
			ENGINE_CORE_INFO("create SDL2 window");
			window = SDL_CreateWindow(data.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, static_cast<int>(data.width), static_cast<int>(data.height), windowFlags | SDL_WINDOW_RESIZABLE);
		}

		context = Context::create(this);
		context->init();

		ENGINE_CORE_ASSERT(window, "SDL_CreateWindow : failed to create a SDL2 window");
		SDL2WindowCount++;
	}

	void SDL2Window::initSDL2(){
		ENGINE_PROFILE_FUNCTION();

		if (SDL2WindowCount == 0){
			ENGINE_CORE_INFO("Initialize SDL2");
			
			ENGINE_PROFILE_SCOPE("SDL2 init");
			bool result = SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS) == 0;
			ENGINE_CORE_ASSERT(result, "failed to init SDL2");
		}
	}

	void SDL2Window::shutdown(){
		ENGINE_PROFILE_FUNCTION();

		ENGINE_CORE_INFO("destroy SDL2 window");
		SDL_DestroyWindow(window);
		SDL2WindowCount--;
	}

	void SDL2Window::shutdownSDL2(){
		ENGINE_PROFILE_FUNCTION();
		if (SDL2WindowCount == 0){
			ENGINE_PROFILE_SCOPE("quit SDL2");
			ENGINE_CORE_INFO("quit SDL2");
			SDL_Quit();
		}
	}

	void SDL2Window::setEventCallback(const EventCallbackFn& callback){
		data.EventCallback = callback;
	}

	void SDL2Window::update(){
		ENGINE_PROFILE_FUNCTION();
		SDL_Event e;

		while (SDL_PollEvent(&e)){

			if (SDLEventsCallback) SDLEventsCallback(&e);

			switch (e.type){
				case SDL_QUIT:{
					WindowCloseEvent event;
					data.EventCallback(event);
					break;
				}

				case SDL_WINDOWEVENT_RESIZED:{
					WindowResizedEvent event(e.window.data1, e.window.data2);
					data.EventCallback(event);
					break;
				}

				case SDL_KEYDOWN:{
					KeyPressedEvent event(static_cast<Key>(e.key.keysym.scancode), e.key.repeat);
					data.EventCallback(event);
					inputs->getKeyState()[static_cast<int>(SDLToEngineKey(e.key.keysym.scancode))] = true;
					break;
				}

				case SDL_KEYUP:{
					KeyReleasedEvent event(static_cast<Key>(e.key.keysym.scancode));
					data.EventCallback(event);
					inputs->getKeyState()[static_cast<int>(SDLToEngineKey(e.key.keysym.scancode))] = false;
					break;
				}

				case SDL_TEXTINPUT:{
					KeyTypedEvent event(SDL_GetScancodeName(e.key.keysym.scancode));
					data.EventCallback(event);
					break;
				}

				case SDL_MOUSEBUTTONDOWN:{
					static float timeLastClick[ENGINE_MOUSE_BUTTON_LAST+1];
					float timeSinceLastClick = static_cast<float>(SDL_GetTicks64())/1000.f - timeLastClick[e.button.button];
					timeLastClick[e.button.button] = static_cast<float>(static_cast<float>(SDL_GetTicks64())/1000.f);

					bool doubleClicked = timeSinceLastClick <= 0.2;

					MouseButtonPressedEvent event(e.button.button, doubleClicked);
					data.EventCallback(event);
					inputs->getButtonStats()[e.button.button] = true;
					break;
				}

				case SDL_MOUSEBUTTONUP:{
					MouseButtonReleasedEvent event(e.button.button);
					data.EventCallback(event);
					inputs->getButtonStats()[e.button.button] = false;
					break;
				}

				case SDL_MOUSEMOTION:{
					MouseMovedEvent event(e.motion.x, e.motion.y);
					data.EventCallback(event);
					inputs->setMousePos(e.motion.x, e.motion.y);
					break;
				}

				case SDL_MOUSEWHEEL:{
					MouseScrolledEvent event(e.wheel.x, e.wheel.y);
					data.EventCallback(event);
					inputs->setMouseWheel(e.wheel.y);
					break;
				}
			}
		}
		context->swapBuffers();
	}

	void SDL2Window::setTitle(const std::string &title){
		SDL_SetWindowTitle(window, title.c_str());
	}
}