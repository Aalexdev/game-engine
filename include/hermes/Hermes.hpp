#pragma once

#include <iostream>
#include <unordered_map>
#include <list>
#include "../horreum/StackAllocator.hpp"
#include <cassert>


#ifdef HERMES_ASSERTS
	#define HERMES_ASSERT(x) assert(x)
#else
	#define HERMES_ASSERT(x)
#endif

class Hermes{
	public:
		using EventID = uint16_t;

		// if the functions/methods return true, that mean that the event ha sbeen handled and should no longer be send to other callbacks
		using EventFn = bool(*)(void*); // event function (the data pointer)
		using EventMt = bool(*)(void*, void*); // event method (the instance pointer and the data pointer)

		union Callback{
			EventFn function;
			EventMt method;
		};

		class DataLayout{
			friend class Hermes;
			public:
				template<typename... Args>
				static DataLayout construct(){
					DataLayout layout;
					layout.add<Args...>();
					return layout;
				}

			private:
				template<typename T, typename A, typename... Args>
				void add(){
					add<T>();
					add<A, Args...>();
				}
				
				template<typename T>
				void add(){
					size += sizeof(T);
				}

				uint16_t size = 0;
		};

		~Hermes();

		static void initialize(uint16_t eventTypeCount, uint32_t bufferSize);

		static EventID registerEvent(const char* name, uint16_t dataSize = 0);
		static EventID registerEvent(const char* name, DataLayout data);

		static EventID getEventIndex(const char* name);

		template<typename... Args>
		static void triggerEvent(const char* name, Args... args){
			triggerEvent(getEventIndex(name), args...);
		}

		template<typename T, typename... Args>
		static void triggerEvent(EventID id, T t, Args... args){
			Hermes& instance = getInstance();
			void* data = _convert(instance.events[id].dataSize, t, args...);
			_triggerEvent(id, data);
		}

		static void triggerEvent(EventID id){
			Hermes& instance = getInstance();
			_triggerEvent(id, nullptr);
		}

		template<typename... Args>
		static void convert(void* ptr, Args&... args){
			size_t offset = 0;
			convertFromVoid(ptr, offset, args...);
		}

		static void subscribe(const char *name, EventFn callback);
		static void subscribe(const char *name, void* subscribedInstance, EventMt callback);
	
		static void subscribe(EventID id, EventFn callback);
		static void subscribe(EventID id, void* subscribedInstance, EventMt callback);

		// this function will call all callbacks of the triggered events and then reset the data buffer
		static void update();


		// ! DEBUG
		static void printEvents();
	
	private:

		static Hermes& getInstance();

		struct EventCallback{

			enum CallbackType{
				Function,
				Method,
			};

			Callback callback;
			CallbackType type;
			void* subscribedInstance = nullptr;
		};

		struct EventType{
			#ifndef NDEBUG
				char* name;
			#endif
			EventID id = 0;
			uint16_t dataSize = 0;
			std::list<EventCallback>* callbacks = nullptr;
		};

		struct EventCall{
			EventID id;
			void* data = nullptr;
		};

		template<typename T, typename... Args>
		static void convertFromVoid(void* ptr, size_t &offset, T &t, Args&... args){
			void* data = static_cast<void*>(static_cast<char*>(ptr) + offset);
			offset += sizeof(T);
			t = *static_cast<T*>(data);
			convertFromVoid(ptr, offset, args...);
		}

		template<typename T>
		static void convertFromVoid(void* ptr, size_t &offset, T &t){
			void* data = static_cast<void*>(static_cast<char*>(ptr) + offset);
			offset += sizeof(T);
			t = *static_cast<T*>(data);
		}


		template<typename... Args>
		static void* _convert(size_t size, Args... args){
			Hermes& instance = getInstance();
			void* ptr = instance.dataBuffer->push(size);
			size_t offset = 0;
			return __convert(ptr, offset, size, args...);
		}

		template<typename T, typename... Args>
		static void* __convert(void* data, size_t &offset, size_t &maxSize, T t, Args... args){
			void* ptr = static_cast<void*>(static_cast<char*>(data) + offset);
			HERMES_ASSERT(offset + sizeof(T) <= maxSize && "data overflow");
			memcpy(ptr, &t, sizeof(T));
			offset+=sizeof(T);
			return __convert(data, offset, maxSize, args...);
		}

		template<typename T>
		static void* __convert(void* data, size_t &offset, size_t &maxSize, T t){
			void* ptr = static_cast<void*>(static_cast<char*>(data) + offset);
			HERMES_ASSERT(offset + sizeof(T) <= maxSize && "data overflow");
			memcpy(ptr, &t, sizeof(T));
			offset+=sizeof(T);
			return data;
		}
		
		static void _triggerEvent(EventID eventID, void* data);
		static bool callCallback(EventCallback &callback, void* data);

		EventType* events;
		StackAllocator *dataBuffer;
		std::list<EventCall> calls;
		std::unordered_map<std::string, EventID> eventMap;
		EventID registeredEventCount = 0;
		EventID maxAvailableEventTypeCount = 0;
};