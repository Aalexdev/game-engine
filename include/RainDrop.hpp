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

	// =============== STRUCTS
	struct Color{
		float r, g, b, a;
	};

	// =============== CLASSES

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

	using Colour = Color;

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
	void RD_API EntityRemoveComponent(Entity entity, uint64_t typeID);

	/**
	 * @brief get if the entity has the given component
	 * 
	 * @param entity the entity to check
	 * @param typeID the id of the component
	 */
	bool RD_API entityHasComponent(Entity entity, uint64_t typeID);
}

#endif // _RAINDROP_HPP_