#include "EventManager.hpp"
#include "Hermes.hpp"

#include <SDL2/SDL.h>

namespace RainDrop{
	struct STDEvents{
		Hermes::EventID windowClosed;
	};

	STDEvents& getSTDEvents(){
		static STDEvents instance;
		return instance;
	}

	void registerEvents(){
		STDEvents& instance = getSTDEvents();

		instance.windowClosed = Hermes::registerEvent("window closed");

	}

	void poolEvents(){
		STDEvents& instance = getSTDEvents();
		
		SDL_Event e;
		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_QUIT:
					Hermes::triggerEvent(instance.windowClosed); break;
			}
		}
	}

}