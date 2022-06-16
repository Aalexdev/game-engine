#include "engine/pch.hpp"
#include "engine/Application.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/filesystem/Filesystem.hpp"
#include "engine/Exception.hpp"

namespace engine{

	static void checkDefinition(const Application::Definition &def){
		if (def.displayCount == 0) throw exceptions::Lenght("invalid display count : must be greater tha zero");
	}

	Ref<Application> Application::create(const Definition &def){
		ENGINE_PROFILE_RECORD();
		ENGINE_PROFILE_BEGIN_SESSION("startup", "EngineProfilingStartup.json");
		checkDefinition(def);

		Ref<Application> app = createRef<Application>(def);
		app->launched = true;
		ENGINE_PROFILE_END_SESSION();
		return app;
	}

	Application::Application(const Definition &def){
		displays.resize(def.displayCount);

		eventHandler = EventHandler::create();

		for (int i=0; i<def.displayCount; i++){
			def.displaysDefinitions[i].app = this;
			if (def.displaysDefinitions[i].eventCallback == nullptr){
				def.displaysDefinitions[i].eventCallback = eventHandler->getEventCallback();
			}
		}

		registerSystemEvents();
		eventHandler->subscribeToEvent("WindowClosed", &onWindowClosedEvent);

		for (uint8_t i=0; i<def.displayCount; i++){
			displays[i] = Display::create(def.displaysDefinitions[i]);
		}
	}
	
	Application::~Application(){

	}

	void Application::run(){
		while (launched){
			// update

			// for each camera : render

			// update displays
			for (auto &display : displays){
				display->update();
			}

			for (auto &displayIt : destroyedDisplays){
				displays.erase(displayIt);
			}
			destroyedDisplays.clear();

			if (displays.empty()) launched = false;
		}
	}

	void Application::onWindowClosedEvent(WindowCloseEvent &e){
		Application* app = e.app;
		auto it = app->displays.begin();
		while (it != app->displays.end()){
			if ((*it).get() == e.getDisplay()){
				app->destroyedDisplays.push_back(it);
				break;
			}
			it++;
		}

	}

	void Application::registerSystemEvents(){
		eventHandler->registerEvent("WindowClosed", static_cast<uint32_t>(EventType::EVENT_WINDOW_CLOSED));
		eventHandler->registerEvent("WindowResized", static_cast<uint32_t>(EventType::EVENT_WINDOW_RESIZED));
		eventHandler->registerEvent("WindowFocused", static_cast<uint32_t>(EventType::EVENT_WINDOW_FOCUSED));
		eventHandler->registerEvent("WindowLostFocus", static_cast<uint32_t>(EventType::EVENT_WINDOW_LOST_FOCUS));
		eventHandler->registerEvent("WindowMoved", static_cast<uint32_t>(EventType::EVENT_WINDOW_MOVED));
		eventHandler->registerEvent("WindowMinimized", static_cast<uint32_t>(EventType::EVENT_WINDOW_MINIMIZED));
		eventHandler->registerEvent("WindowMaximized", static_cast<uint32_t>(EventType::EVENT_WINDOW_MAXIMIZED));
		eventHandler->registerEvent("AppTick", static_cast<uint32_t>(EventType::EVENT_APP_TICK));
		eventHandler->registerEvent("KeyPressed", static_cast<uint32_t>(EventType::EVENT_KEY_PRESSED));
		eventHandler->registerEvent("KeyTyped", static_cast<uint32_t>(EventType::EVENT_KEY_TYPED));
		eventHandler->registerEvent("KeyReleased", static_cast<uint32_t>(EventType::EVENT_KEY_RELEASED));
		eventHandler->registerEvent("MouseButtonReleased", static_cast<uint32_t>(EventType::EVENT_MOUSE_BUTTON_RELEASED));
		eventHandler->registerEvent("MouseButtonPressed", static_cast<uint32_t>(EventType::EVENT_MOUSE_BUTTON_PRESSED));
		eventHandler->registerEvent("MouseMoved", static_cast<uint32_t>(EventType::EVENT_MOUSE_MOVED));
		eventHandler->registerEvent("MouseScolled", static_cast<uint32_t>(EventType::EVENT_MOUSE_SCROLLED));
	}
}