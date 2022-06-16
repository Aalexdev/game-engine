#pragma once

#include "core.hpp"
#include "displays/Display.hpp"
#include "layer/LayerStack.hpp"
#include "layer/ImGuiLayer/ImGuiLayer.hpp"
#include "renderer/Renderer.hpp"

#include "events/Event.hpp"
#include "events/WindowEvent.hpp"
#include "events/Input.hpp"
#include "events/ApplicationEvent.hpp"
#include "events/EventHandler.hpp"

// std
#include <condition_variable>
#include <mutex>

namespace engine{
	class ENGINE_API Application{
		friend class EventHandler;
		public:
			struct ENGINE_API Definition{
				uint8_t displayCount = 0;
				Display::Definition* displaysDefinitions = nullptr;
			};

			static Ref<Application> create(const Definition &def);
			Application(const Definition &def);
			~Application();

			void run();

		private:
			static void onWindowClosedEvent(WindowCloseEvent &e);
			void registerSystemEvents();

			bool launched;
			std::vector<Ref<Display>> displays;
			std::list<std::vector<Ref<Display>>::iterator> destroyedDisplays;
			Ref<EventHandler> eventHandler;

	};
}