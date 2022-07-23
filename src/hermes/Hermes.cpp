#include "hermes/Hermes.hpp"
#include "horreum/Horreum.hpp"
#include <cassert>
#include <cstring>


Hermes& Hermes::getInstance(){
	static Hermes instance;
	return instance;
}

void Hermes::initialize(uint16_t eventTypeCount, uint32_t bufferSize){
	Hermes &instance = getInstance();
	instance.maxAvailableEventTypeCount = eventTypeCount;
	instance.events = static_cast<EventType*>(HRM_MALLOC(sizeof(EventType) * eventTypeCount));

	for (int i=0; i<eventTypeCount; i++){
		instance.events[i].callbacks = new std::list<EventCallback>();
		
	}

	instance.dataBuffer = new StackAllocator(bufferSize);
}

Hermes::~Hermes(){
	#ifndef NDEBUG
		for (int i=0; i<registeredEventCount; i++){
			HRM_FREE(events[i].name);
		}
	#endif

	for (int i=0; i<maxAvailableEventTypeCount; i++){
		delete events[i].callbacks;
	}

	HRM_FREE(events);
	delete dataBuffer;
}

Hermes::EventID Hermes::registerEvent(const char* name, uint16_t dataSize){
	Hermes& instance = getInstance();
	HERMES_ASSERT(instance.registeredEventCount <= instance.maxAvailableEventTypeCount && "event type overflow");

	EventID &registeredEventCount = instance.registeredEventCount;

	// check if the name is already used
	{
		auto iterator = instance.eventMap.find(name);
		if (iterator != instance.eventMap.end()){
			return iterator->second;
		}
	}

	// create the evnt type from the given informations
	EventType &event = instance.events[registeredEventCount];
	#ifndef NDEBUG
		event.name = static_cast<char*>(HRM_MALLOC(sizeof(char) * strlen(name)));
		strcpy(event.name, name);
	#endif
	event.id = registeredEventCount;
	event.dataSize = dataSize;

	EventID id = registeredEventCount;
	registeredEventCount++;

	instance.eventMap[name] = id;
	return id;
}

Hermes::EventID Hermes::registerEvent(const char* name, DataLayout data){
	return registerEvent(name, data.size);
}

void Hermes::_triggerEvent(EventID eventID, void* data){
	Hermes& instance = getInstance();
	HERMES_ASSERT(eventID < instance.registeredEventCount && "event type overflow");
	EventCall call;
	call.id = eventID;
	call.data = data;

	instance.calls.push_back(call);
}

bool Hermes::callCallback(EventCallback &callback, void* data){
	switch (callback.type){
		case EventCallback::Function: return callback.callback.function(data);
		case EventCallback::Method: return callback.callback.method(callback.subscribedInstance, data);
	}
	return false;
}

void Hermes::printEvents(){
	Hermes& instance = getInstance();

	#ifndef NDEBUG
	for (int i=0; i<instance.registeredEventCount; i++){
		auto &event = instance.events[i];

		printf("name : %s, id : %d, size : %d\n", event.name, i, event.dataSize);
	}
	#endif
}

Hermes::EventID Hermes::getEventIndex(const char* name){
	Hermes& instance = getInstance();
	auto iterator = instance.eventMap.find(name);
	HERMES_ASSERT(iterator != instance.eventMap.end() && "unknown event name");
	return iterator->second;
}

void Hermes::update(){
	Hermes &instance = getInstance();
	for (auto &c : instance.calls){
		EventType& event = instance.events[c.id];
		
		for (auto &callback : *event.callbacks){
			if (callCallback(callback, c.data)) break;
		}
	}

	instance.calls.clear();
	instance.dataBuffer->clear();
}

void Hermes::subscribe(const char *name, EventFn callback){subscribe(getInstance().getEventIndex(name), callback);}
void Hermes::subscribe(const char *name, void* subscribedInstance, EventMt callback){subscribe(getInstance().getEventIndex(name), subscribedInstance, callback);}

void Hermes::subscribe(EventID id, EventFn callback){
	Hermes &instance = getInstance();
	HERMES_ASSERT(instance.registeredEventCount >= id && "event type overflow");

	EventType& event = instance.events[id];
	EventCallback cb;
	cb.callback.function = callback;
	cb.type = cb.Function;
	event.callbacks->push_back(cb);
}

void Hermes::subscribe(EventID id, void* subscribedInstance, EventMt callback){
	Hermes &instance = getInstance();
	HERMES_ASSERT(instance.registeredEventCount >= id && "event type overflow");

	EventType& event = instance.events[id];
	EventCallback cb;
	cb.callback.method = callback;
	cb.type = cb.Method;
	event.callbacks->push_back(cb);
}