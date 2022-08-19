#include "Odin.hpp"
#include "core.hpp"
#include "Fovea.h"
#include "Gramophone.hpp"

#include "RainDrop.hpp"

#define RD_TRHOW_EXCEPT(what, why) throw RainDrop::Exception(what, __func__, why);

namespace RainDrop{

	// ==== exception ====
	#pragma region exception
		Exception::Exception(const char *what, const char *where, const char *why) : m_what{what}, m_where{where}, m_why{why}{}
		const char* Exception::what() const {return m_what;}
		const char* Exception::where() const {return m_where;}
		const char* Exception::why() const {return m_why;}
	#pragma endregion

	// === window ===
	void createWindow(){
		Core& instance = getInstance();
		
		instance.window = SDL_CreateWindow("RainDrop", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);
	}

	void RD_API initialize(){
		// FoveaInitialize();
		Gramophone::initialize();
		Odin::initialize();
	}
}