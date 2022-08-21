/**
 * MIT License
 * 
 * Copyright (c) 2022 Alexis Magnier
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _RAINDROP_HPP_
#define _RAINDROP_HPP_

#ifdef RD_BUILD_DYNAMIC
	#if defined(__WIN32__) || defined(__WINRT__) || defined(__CYGWIN__) || defined(__OS2__)
		#ifdef RD_BUILD
			#define RD_API __declspec(dllexport)
		#else
			#define RD_API __declspec(dllimport)
		#endif
	#else
		#define RD_API
	#endif
#else
	#define RD_API
#endif

#ifndef _STDINT_H
	typedef char int8_t;
	typedef unsigned char uint8_t;
	typedef signed char sint8_t;
	typedef short int16_t;
	typedef unsigned short uint16_t;
	typedef signed short sint16_t;
	typedef int int32_t;
	typedef unsigned int uint32_t;
	typedef signed int sint32_t;
	typedef long long int64_t;
	typedef unsigned long long uint64_t;
	typedef signed long long sint64_t;
#endif

namespace RainDrop{
	using EntityID = uint32_t;
	using EventID = uint16_t;

	class RD_API Exception{
		public:
			Exception(const char *what, const char *where, const char *why);

			const char* what() const;
			const char* where() const;
			const char* why() const;
		
		private:
			const char* m_what;
			const char* m_where;
			const char* m_why;
	};

	// ==========================================================
	// ==                        TYPES                         ==
	// ==========================================================

	// =============== ENUMS

	// events
	enum class MouseButton{
		Left,
		Middle,
		Right,
		X1,
		X2,
	};

	enum class Key{
		K_NONE,
		K_SPACE,
		K_COMMA,
		K_MINUS,
		K_PERIOD,
		K_SLASH,
		K_0,
		K_1,
		K_2,
		K_3,
		K_4,
		K_5,
		K_6,
		K_7,
		K_8,
		K_9,
		K_SEMICOLON,
		K_EQUAL,
		K_A,
		K_B,
		K_C,
		K_D,
		K_E,
		K_F,
		K_G,
		K_H,
		K_I,
		K_J,
		K_K,
		K_L,
		K_M,
		K_N,
		K_O,
		K_P,
		K_Q,
		K_R,
		K_S,
		K_T,
		K_U,
		K_V,
		K_W,
		K_X,
		K_Y,
		K_Z,
		K_LEFT_BRACKET,
		K_BACKSLASH,
		K_RIGHT_BRACKET,
		K_WORLD_1,
		K_WORLD_2,
		K_ESCAPE,
		K_ENTER,
		K_TAB,
		K_BACKSPACE,
		K_INSERT,
		K_DELETE,
		K_RIGHT,
		K_LEFT,
		K_DOWN,
		K_UP,
		K_PAGE_UP,
		K_PAGE_DOWN,
		K_HOME,
		K_END,
		K_CAPS_LOCK,
		K_SCROLL_LOCK,
		K_NUM_LOCK,
		K_PRINT_SCREEN,
		K_PAUSE,
		K_F1,
		K_F2,
		K_F3,
		K_F4,
		K_F5,
		K_F6,
		K_F7,
		K_F8,
		K_F9,
		K_F10,
		K_F11,
		K_F12,
		K_F13,
		K_F14,
		K_F15,
		K_F16,
		K_F17,
		K_F18,
		K_F19,
		K_F20,
		K_F21,
		K_F22,
		K_F23,
		K_F24,
		K_KP_0,
		K_KP_1,
		K_KP_2,
		K_KP_3,
		K_KP_4,
		K_KP_5,
		K_KP_6,
		K_KP_7,
		K_KP_8,
		K_KP_9,
		K_KP_DECIMAL,
		K_KP_DIVIDE,
		K_KP_MULTIPLY,
		K_KP_SUBTRACT,
		K_KP_ADD,
		K_KP_ENTER,
		K_KP_EQUAL,
		K_LEFT_SHIFT,
		K_LEFT_CONTROL,
		K_LEFT_ALT,
		K_RIGHT_SHIFT,
		K_RIGHT_CONTROL,
		K_RIGHT_ALT,
		K_MENU,
		K_MAX,
	};

	// =============== STRUCTS
	struct Color{
		float r, g, b, a;
	};

	template<typename T>
	struct vec2{
		T x, y;
	};

	template<typename T>
	struct vec3{
		T x, y, z;
	};

	template<typename T>
	struct vec4{
		T x, y, z, w;
	};

	// =============== CLASSES
	class Entity;

	/**
	 * @brief initialize the rainDrop engine and all of it subsystems
	 * 
	 */
	void RD_API initialize();
	
	/**
	 * @brief shutdown the rainDrop engine, some of the subsystem will be automaticaly closed at the end of the program
	 * 
	 */
	void RD_API shutdown();

	/**
	 * @brief start the mainloop
	 * 
	 */
	void RD_API run();

	// ==========================================================
	// ==                       WINDOW                         ==
	// ==========================================================

	/**
	 * @brief set the size of the window in pixles
	 * 
	 * @param size a vector 2D that contain the size of the window
	 */
	void RD_API setWindowSize(const vec2<uint32_t> &size);

	/**
	 * @brief move the window to the required position
	 * 
	 * @param pos the position in pixels of the top left corner of the window, relative to the top left corner of the screen
	 */
	void RD_API setWindowPosition(const vec2<uint32_t> &pos);

	/**
	 * @brief set the title of the window
	 * 
	 * @param title the title of the window as a string (UTF-8 encoding)
	 */
	void RD_API setWindowTitle(const char *title);

	/**
	 * @brief set the window into fullscreen mod
	 * 
	 */
	void RD_API setWindowFullscreen();

	/**
	 * @brief set the window into normal window mod 
	 * 
	 */
	void RD_API unsetWindowFullscreen();
	
	/**
	 * @brief make the window fullsceen and resize it to the screen size
	 * 
	 */
	void RD_API setWindowDesktopFullscreen();

	/**
	 * @brief set if the window is resizable or not
	 * @param resizable is the window resizable
	 */
	void RD_API setWindowResizable(bool resizable);

	// ==========================================================
	// ==                       EVENTS                         ==
	// ==========================================================

	EventID RD_API registerEvent(const char* name, uint32_t dataSize = 0);

	EventID RD_API getEventID(const char* name);

	void RD_API subscribeEvent(const char* name, bool(*FNcallback)(void*));

	void RD_API subscribeEvent(const char* name, void* instance, bool(*MTcallback)(void*, void*));
	
	void RD_API subscribeEvent(EventID id, bool(*FNcallback)(void*));

	void RD_API subscribeEvent(EventID id, void* instance, bool(*MTcallback)(void*, void*));

	void RD_API unsubscribeEvent(const char* name, bool(*FNcallback)(void*));

	void RD_API unsubscribeEvent(const char* name, bool(*MTcallback)(void*, void*));
	
	void RD_API unsubscribeEvent(EventID id, bool(*FNcallback)(void*));

	void RD_API unsubscribeEvent(EventID id, bool(*MTcallback)(void*, void*));

	// ==========================================================
	// ==                       RENDER                         ==
	// ==========================================================

	void RD_API setClearColor(const Color &color);

	// ==========================================================
	// ==                         ECS                          ==
	// ==========================================================

	/**
	 * @brief create a new entity into the ECS and return it index
	 * @return Entity 
	 */
	Entity RD_API createEntity();
	
	/**
	 * @brief destroy an entity from the ECS
	 * @param entity the entity to destroy
	 */
	void RD_API destroyEntity(Entity entity);

	/**
	 * @brief add to the entity, the given component
	 * 
	 * @param entity the entity to add the component
	 * @param component a pointer to the component data
	 * @param typeID the id of the component
	 */
	void RD_API entityAddComponent(Entity entity, void* component, uint64_t typeID);

	/**
	 * @brief remove from the entity, the given component
	 * 
	 * @param entity the entity to remove from
	 * @param typeID the id of the component
	 */
	void RD_API entityRemoveComponent(Entity entity, uint64_t typeID);

	/**
	 * @brief get if the entity has the given component
	 * 
	 * @param entity the entity to check
	 * @param typeID the id of the component
	 */
	bool RD_API entityHasComponent(Entity entity, uint64_t typeID);

	// ==========================================================
	// ==                TYPES WITH DEPENDENCES                ==
	// ==========================================================

	class Entity{
		public:
			Entity(const Entity &) = default;
			Entity(EntityID id) : id{id}{}

			EntityID getUID() const {return id;}

			template<typename T>
			void addComponent(T t={}){
				entityAddComponent(id, &t, typeid(T).hash_code());
			}

			template<typename T>
			void removeComponent(){
				entityRemoveComponent(id, typeid(T).hash_code());
			}

			template<typename T>
			bool hasComponent(){
				return entityHasComponent(id, typeid(T).hash_code());
			}

		private:
			EntityID id;

	};
}

#endif // _RAINDROP_HPP_