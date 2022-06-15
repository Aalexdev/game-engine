#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "Event.hpp"

namespace engine{
	class ENGINE_API EventHandler{
		public:
			static Ref<EventHandler> create();
			EventHandler();
			~EventHandler();

			EventFn getEventCallback();

			static void OnEvent(Event &e);

			template<typename T>
			void subscribeToEvent(const std::string &eventName, const T &fn){
				auto it = nameToEventMap.find(eventName);
				if (it == nameToEventMap.end()) return;
				it->second->callbacks.insert((EventFn)(fn));
			}

			void registerEvent(const std::string &name, uint32_t id);
			void registerEvent(const std::string &name);

		private:
			struct EventType{
				std::string name;
				uint32_t id;
				std::set<EventFn> callbacks;
			};

			uint32_t eventTypesCount = 0;
			std::queue<uint32_t> unusedIDs;
			std::vector<Ref<EventType>> eventTypes;
			std::unordered_map<std::string, Ref<EventType>> nameToEventMap;

			void dispatchEvent(Event &e);
			void reserveEventTypes(uint32_t count = 50);
	};
}