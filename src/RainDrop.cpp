#include "Odin.hpp"
#include "core.hpp"
#include "Fovea.h"
#include "Gramophone.hpp"
#include "Hermes.hpp"
#include "EventManager.hpp"
#include "RainDrop.hpp"

#define RD_TRHOW_EXCEPT(what, why) throw RainDrop::Exception(what, __func__, why);

namespace RainDrop{

	// ==== exception ====
	Exception::Exception(const char *what, const char *where, const char *why) : m_what{what}, m_where{where}, m_why{why}{}
	const char* Exception::what() const {return m_what;}
	const char* Exception::where() const {return m_where;}
	const char* Exception::why() const {return m_why;}

	// === window ===
	void createWindow(){
		Core& instance = getInstance();
		
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		instance.window = SDL_CreateWindow("RainDrop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

		if (!instance.window){
			throw SDL_GetError();
		}
	}

	void shutdownWindow(){
		Core& instance = getInstance();
		
		if (instance.window) SDL_DestroyWindow(instance.window);
		instance.window = nullptr;
		SDL_Quit();
	}

	// === ECS ===

	void initializeECS(){
		ECS::Coordinator& coordinator = getInstance().scene;
		coordinator.Init();

		// REGISTER DEFAULT SYSTEMS AND COMPONENTS
	}

	// === EVENTS ===
	bool OnWindowClosed(void*){
		getInstance().launched = false;
		return false;
	}

	// === API functions ===

	void RD_API initialize(){
		Core& instance = getInstance();

		try {
			createWindow();
		} catch (const char *err){
			RD_TRHOW_EXCEPT("failed to initialized window", err);
		}

		FoveaInitialize(static_cast<void*>(instance.window));
		Gramophone::initialize();
		Odin::initialize();
		Hermes::initialize(150, 1500);

		initializeECS();
		registerEvents();

		Hermes::subscribe("on window closed", &OnWindowClosed);

		instance.launched = true;
	}

	void RD_API shutdown(){
		shutdownWindow();
	}

	void RD_API run(){
		Core& instance = getInstance();
		while (instance.launched){

			poolEvents();
			FoveaBeginFrame();

			FoveaEndFrame();
		}
	}

	// ====================================== Window

	void RD_API setWindowSize(const vec2<uint32_t> &size){
		SDL_SetWindowSize(getInstance().window, static_cast<int>(size.x), static_cast<int>(size.y));
		FoveaOnWindowResized(size.x, size.y);
	}

	void RD_API setWindowPosition(const vec2<uint32_t> &pos){
		SDL_SetWindowPosition(getInstance().window, static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	void RD_API setWindowTitle(const char *title){
		SDL_SetWindowTitle(getInstance().window, title);
	}

	void RD_API setWindowFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, SDL_WINDOW_FULLSCREEN);
	}

	void RD_API unsetWindowFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, 0);
	}

	void RD_API setWindowDesktopFullscreen(){
		SDL_SetWindowFullscreen(getInstance().window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	void RD_API setWindowResizable(bool resizable){
		SDL_SetWindowResizable(getInstance().window, static_cast<SDL_bool>(resizable));
	}

	// ====================================== Render

	void RD_API setClearColor(const Color &color){
		FoveaSetClearColor({color.r, color.g, color.b, color.a});
	}

	// ====================================== ECS

	Entity RD_API createEntity(){
		return getInstance().scene.CreateEntity();
	}

	void RD_API destroyEntity(Entity entity){
		return getInstance().scene.DestroyEntity(entity.getUID());
	}

	void RD_API entityAddComponent(Entity entity, void* component, uint64_t typeID){
		getInstance().scene.AddComponent(entity.getUID(), component, typeID);
	}

	void RD_API entityRemoveComponent(Entity entity, uint64_t typeID){
		getInstance().scene.RemoveComponent(entity.getUID(), typeID);
	}

	bool RD_API entityHasComponent(Entity entity, uint64_t typeID){
		return getInstance().scene.HasComponent(entity.getUID(), typeID);
	}
}