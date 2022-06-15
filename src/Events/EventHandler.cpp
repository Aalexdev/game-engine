#include "engine/pch.hpp"
#include "engine/events/EventHandler.hpp"
#include "engine/Application.hpp"

namespace engine{
	
	Ref<EventHandler> EventHandler::create(){
		return createRef<EventHandler>();
	}

	EventHandler::EventHandler(){
		reserveEventTypes(250);
	}

	EventHandler::~EventHandler(){

	}

	EventFn EventHandler::getEventCallback(){
		return &OnEvent;
	}

	void EventHandler::dispatchEvent(Event &e){
		auto &type = eventTypes[e.getEventType()];
		for (auto fnc : type->callbacks){
			fnc(e);
		}
	}

	void EventHandler::OnEvent(Event &e){
		ENGINE_CORE_TRACE(e.toString());
		Application *app = e.app;
		app->eventHandler->dispatchEvent(e);
	}

	void EventHandler::registerEvent(const std::string &name, uint32_t id){
		if (nameToEventMap.find(name) != nameToEventMap.end()) return;

		Ref<EventType> type = createRef<EventType>();
		type->name = name;
		type->id = id;

		nameToEventMap[name] = type;
		eventTypes[id] = type;
	}

	void EventHandler::registerEvent(const std::string &name){
		if (nameToEventMap.find(name) != nameToEventMap.end()) return;

		if (unusedIDs.empty()){
			reserveEventTypes();
		}

		Ref<EventType> type = createRef<EventType>();
		type->name = name;
		type->id = unusedIDs.front();
		unusedIDs.pop();

		nameToEventMap[name] = type;
		eventTypes[type->id] = type;
	}

	void EventHandler::reserveEventTypes(uint32_t count){
		for (uint32_t i=0; i<count; i++){
			eventTypes.push_back(nullptr);
			unusedIDs.push(eventTypesCount+i);
		}
	}
}