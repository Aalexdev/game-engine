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

#include <typeinfo>
#include <stdint.h>
#include <assert.h>
#include <set>
#include <bitset>

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

#define RD_MAT(r, c) template<typename T> struct mat##r##x##c{RainDrop::vec##c<T> row[r];};

namespace RainDrop{
	static constexpr uint64_t ECS_MAX_COMPONENT = 32;
	static constexpr uint64_t ECS_MAX_ENTITIES = 1024;
	static constexpr uint64_t DEPTH_BUFFER = -1;

	using EntityID = uint32_t;
	using EventID = uint16_t;
	using ECSSignature = std::bitset<ECS_MAX_COMPONENT>; 
	using ShaderID = uint64_t;
	using RenderTargetID = uint64_t;
	using DescriptorSetID = uint64_t;
	using TextureID = uint64_t;
	using SoundID = uint64_t;
	using SoundSourceID = uint64_t;

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

	// === events
	// mouse buttons
	enum class MouseButton{
		Left,
		Middle,
		Right,
		X1,
		X2,
		MAX,
	};

	// keyboard keys
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

	enum class ReverbType{
		Default,
		Paddedcell,
		Room,
		Bathroom,
		Livingroom,
		Stoneroomn,
		Auditorium,
		Concerthall,
		Cave,
		Arena,
		Hangar,
		Carpettedhallway,
		Hallway,
		Forest,
		City,
		Mountains,
		Quarry,
		Plain,
		ParkingLot,
		SewerPipe,
		Drugged,
		Dizzy,
		Psychotic,

		Castle_smallRoomn,
		Castle_shortPassage,
		Castle_mediumRoom,
		Castle_longPassage,
		Castle_largeRoom,
		Castle_hall,
		Castle_cupboard,
		Castle_courtyard,
		Castle_alcove,
		
		Factory_alcove,
		Factory_shortPassage,
		Factory_mediumRoom,
		Factory_longPassage,
		Factory_largeRoom,
		Factory_hall,
		Factory_cupboard,
		Factory_courtyard,
		Factory_smallRoom,
		
		IcePalace_alcove,
		IcePalace_shortPassage,
		IcePalace_mediumRoom,
		IcePalace_longPassage,
		IcePalace_largeRoom,
		IcePalace_hall,
		IcePalace_cupboard,
		IcePalace_courtyard,
		IcePalace_smallRoom,

		SpaceStation_alcove,
		SpaceStation_shortPassage,
		SpaceStation_mediumRoom,
		SpaceStation_longPassage,
		SpaceStation_largeRoom,
		SpaceStation_hall,
		SpaceStation_cupboard,
		SpaceStation_smallRoom,

		Wooden_alcove,
		Wooden_shortPassage,
		Wooden_mediumRoom,
		Wooden_longPassage,
		Wooden_largeRoom,
		Wooden_hall,
		Wooden_cupboard,
		Wooden_courtyard,
		Wooden_smallRoom,

		Sport_emptyStadium,
		Sport_squashCourt,
		Sport_smallSwimmingPool,
		Sport_largeSwimmingPool,
		Sport_gymnasium,
		Sport_fullStadium,
		Sport_stadiumTannoy,
		
		Prefab_workshop,
		Prefab_schoolRoom,
		Prefab_practisRoom,
		Prefab_outHouse,
		Prefab_Caravan,

		Dome_tomb,
		Dome_saintPauls,
		
		Pipe_small,
		Pipe_longThin,
		Pipe_large,
		Pipe_resonant,
		
		Outdoors_backyard,
		Outdoors_rollingPlains,
		Outdoors_deepCanyon,
		Outdoors_creek,
		Outdoors_valley,

		Mood_heaven,
		Mood_hell,
		Mood_memory,
		
		Driving_commentator,
		Driving_pitGarage,
		Driving_fullGrandStand,
		Driving_emptyGrandStand,
		Driving_tunnel,
		Driving_incarLuxury,
		Driving_incarSports,
		Driving_incarRacer,

		City_streets,
		City_subway,
		City_museum,
		City_library,
		City_underPass,
		City_abandoned,

		DustyRoom,
		Chapel,
		SmallWaterRoom,
	};

	enum class Topology{
		Quad = 0,
		Trigone = 1,
		Line = 2,
	};

	enum class TypeFormat{
		Float = 0,
		Float_vec2 = 1,
		Float_vec3 = 2,
		Float_vec4 = 3,
		Int = 4,
		Int_vec2 = 5,
		Int_vec3 = 6,
		Int_vec4 = 7,
		Uint = 8,
		Uint_vec2 = 9,
		Uint_vec3 = 10,
		Uint_vec4 = 11,
	};

	enum class TextureFormat{
		R8 = 0,
		R8G8 = 1,
		R8G8B8 = 2,
		R8G8B8A8 = 3,
		R16 = 4,
		R16G16 = 5,
		R16G16B16 = 6,
		R16G16B16A16 = 7,
		R32 = 8,
		R32G32 = 9,
		R32G32B32 = 10,
		R32G32B32A32 = 11,
		R64 = 12,
		R64G64 = 13,
		R64G64B64 = 14,
		R64G64B64A64 = 15,
		R4G4 = 16,
		R4G4B4A4 = 17,
		R5G5B5A1 = 18,
		R5B6G5 = 19,
		D32 = 20,
		D32_S8 = 21,
		D24_S8 = 22,
	};

	enum class Tiling{
		Linear = 0,
		Nearest = 1,
	};

	enum class Sample{
		Sample_1 = 0,
		Sample_2 = 1,
		Sample_4 = 2,
		Sample_8 = 3,
		Sample_16 = 4,
		Sample_32 = 5,
		Sample_64 = 6,
	};

	enum class ShaderStage{
		Vertex = 1 << 0,
		Geometry = 1 << 1,
		Fragment = 1 << 2,
	};

	enum class TextureAdressMode{
		Repeat = 0,
		MirroredRepeat = 1,
		ClampToEdge = 2,
		ClampToBorder = 3,
	};

	enum class DescriptorType{
		TextureID = 0,
		Buffer = 1,
	};

	enum class PolygoneMode{
		Fill,
		Line,
		Point,
	};

	// =============== STRUCTS
	struct Color{
		float r, g, b, a;
	};

	// math
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
	class RD_API Entity;

	class RD_API AssetData{
		public:
			AssetData(){}
			~AssetData(){
				if (data) free(data);
			}

			template<typename... Args>
			AssetData(Args&&... args){
				set(args...);
			}

			template<typename... Args>
			void get(Args&... args) const{
				assert(getArgsSize<Args...>() <= size && "the arguments total size is greater than the data");
				size_t offset = 0;
				getData(data, offset, args...);
			}

			template<typename... Args>
			void set(Args&&... args){
				reset();
				size = getArgsSize<Args...>();
				data = malloc(size);
				setData(size, args...);
			}

			void reset(){
				if (data) free(data);
				data = nullptr;
				size = 0;
			}

			size_t getSize() const{
				return size;
			}
		
		private:
			template<typename... Args>
			void* setData(size_t size, Args... args){
				void* ptr = data;
				size_t offset = 0;
				return _setData(ptr, offset, size, args...);
			}

			template<typename T, typename... Args>
			static void* _setData(void* data, size_t &offset, size_t &maxSize, T t, Args... args){
				void* ptr = static_cast<void*>(static_cast<char*>(data) + offset);
				assert(offset + sizeof(T) <= maxSize && "data overflow");
				memcpy(ptr, &t, sizeof(T));
				offset+=sizeof(T);
				return _setData(data, offset, maxSize, args...);
			}

			template<typename T>
			static void* _setData(void* data, size_t &offset, size_t &maxSize, T t){
				void* ptr = static_cast<void*>(static_cast<char*>(data) + offset);
				assert(offset + sizeof(T) <= maxSize && "data overflow");
				memcpy(ptr, &t, sizeof(T));
				offset+=sizeof(T);
				return data;
			}

			template<typename T, typename... Args>
			static void getData(void* ptr, size_t& offset, T& t, Args&... args){
				void* data = static_cast<void*>(static_cast<char*>(ptr) + offset);
				offset += sizeof(T);
				t = *static_cast<T*>(data);
				getData(ptr, offset, args...);
			}

			template<typename T>
			static void getData(void* ptr, size_t& offset, T& t){
				void* data = static_cast<void*>(static_cast<char*>(ptr) + offset);
				offset += sizeof(T);
				t = *static_cast<T*>(data);
			}

			template<typename... Args>
			static size_t getArgsSize(){
				size_t totalSize = 0;
				addArgsSize<Args...>(totalSize);
				return totalSize;
			}

			template<typename T, typename A, typename... Args>
			static void addArgsSize(size_t &size){
				addArgsSize<T>(size);
				addArgsSize<A, Args...>(size);
			}

			template<typename T>
			static void addArgsSize(size_t &size){
				size += sizeof(T);
			}

			size_t size = 0;
			void* data = nullptr;
			
	};

	class RD_API AssetFactory{
		public:
			AssetFactory() = default;
			virtual ~AssetFactory() = default;

			virtual size_t getTypeHash() const = 0;
			virtual void* create(const AssetData &data) = 0;
			virtual void destroy(void* asset) = 0;
	};

	template<typename T>
	class RD_API AssetReference{
		public:
			AssetReference() = default;
			AssetReference(const AssetReference<T>& other){set(other);}
			void operator=(const AssetReference<T>& other){set(other);}

			~AssetReference(){reset();}

			void reset(){
				if (asset){
					(*count)--;
					if (*count == 0){
						removeAsset(asset);
					}
				}
				asset = nullptr;
				count = nullptr;
				removeAsset = nullptr;
			}

			T* operator->() const {return asset;}
			T& operator&() {return *asset;}
			operator bool() const {return asset != nullptr;}
			bool operator==(const AssetReference<T> &other) {return asset == other.asset;}
			bool operator!=(const AssetReference<T> &other) {return asset != other.asset;}

		private:
			void set(const AssetReference<T>& other){
				count = other.count;
				(*count)++;
				asset = other.asset;
				removeAsset = other.removeAsset;
			}

			// callback when the count get down to zero
			void (*removeAsset)(void*) = nullptr;
			uint16_t* count = nullptr;
			T* asset = nullptr;

			template<typename G, typename... Args>
			friend AssetReference<G> createAsset(const char* name, Args&... args);
	};

	class RD_API ECSSystem{
        public:
            std::set<EntityID> entities;
    };

	class RD_API ShaderCreateInfo{
		public:
			struct VertexAttribute{
				TypeFormat format;
				uint32_t offset;
			};

			void setVertexFilepath(const char* filepath){
				vertexFilepath = filepath;
			}
			
			void setFragmentFilepath(const char* filepath){
				fragmentFilepath = filepath;
			}

			void setGeometryFilepath(const char* filepath){
				geometryFilepath = filepath;
			}

			template<typename T>
			void setPushConstant(){
				pushConstantSize = static_cast<uint32_t>(sizeof(T));
			}

			void setSampleCount(Sample sample){
				this->sample = sample;
			}

			void setRenderTarget(RenderTargetID renderTargetID){
				this->renderTargetID = renderTargetID;
			}

			void setBaseShader(ShaderID base){
				this->base = base;
			}

			void setVertexAttributes(VertexAttribute* attributes, uint32_t attributeCount){
				vertexAttributes = attributes;
				vertexAttributeCount = attributeCount;
			}

			void setDescriptorSets(DescriptorSetID* descriptors, uint32_t descriptorCount){
				descriptorSets = descriptors;
				descriptorSetCount = descriptorCount;
			}

			void enableBlending(bool blending){
				this->blending = blending;
			}

			void enableDepthDest(bool depthDest){
				this->depthTest = depthTest;
			}

			void setLineWidth(float width){
				this->lineWidth = width;
			}

			void setPolygoneMode(PolygoneMode mode){
				polygoneMode = mode;
			}

			void setVertexInputSize(uint32_t size){
				vertexInputSize = size;
			}

		private:
			std::string vertexFilepath;
			std::string fragmentFilepath;
			std::string geometryFilepath;

			uint32_t pushConstantSize = 0;

			Sample sample = Sample::Sample_1;
			RenderTargetID renderTargetID = -1;
			ShaderID base = -1;

			uint32_t vertexInputSize = 0;
			VertexAttribute* vertexAttributes = nullptr;
			uint32_t vertexAttributeCount = 0;

			DescriptorSetID* descriptorSets = nullptr;
			uint32_t descriptorSetCount = 0;

			PolygoneMode polygoneMode;
			bool blending = false;
			bool depthTest = true;
			float lineWidth = 1.f;

			friend ShaderID RD_API createShader(ShaderCreateInfo &info);
	};

	class RD_API RenderTargetCreateInfo{
		public:
			struct Attachment{
				TextureFormat format;
				Tiling tiling;
				Sample sample;
				Color clearColor;
			};

			void enableDepthStencil(TextureFormat format){
				depthFormat = format;
			}

			void setDepthStencilClearValues(float depth, uint32_t stencil){
				depthClearValue = depth;
				stencilClearValue = stencil;
			}

			void setSize(vec2<uint32_t> size){
				this->size = size;
			}

			void setAttachments(Attachment* attachments, uint32_t attachmentCount){
				colorAttachments = attachments;
				colorAttachmentCount = attachmentCount;
			}

		private:
			uint32_t colorAttachmentCount = 0;
			Attachment* colorAttachments = nullptr;

			TextureFormat depthFormat;
			bool depthStencilEnabled = false;
			float depthClearValue;
			uint32_t stencilClearValue;
			vec2<uint32_t> size;

			friend RenderTargetID RD_API createRenderTarget(RenderTargetCreateInfo  &info);
	};

	class RD_API DescriptorSetCreateInfo{
		public:
			struct DescriptorInfo{
				uint32_t binding;
				int stage;
				DescriptorType type;

				uint32_t bufferSize;
				uint32_t textureCount;
				TextureID* textures;
			};

			void setDescriptors(DescriptorInfo* descriptors, uint32_t descriptorCount){
				this->descriptors = descriptors;
				this->descriptorCount = descriptorCount;
			}

			void setSetCount(uint32_t setCount){
				this->setCount = setCount;
			}

		private:
			DescriptorInfo* descriptors = nullptr;
			uint32_t descriptorCount = 0;
			uint32_t setCount = 0;

			friend DescriptorSetID createDescriptorSet(DescriptorSetCreateInfo& info);
	};

	class RD_API TextureCreateInfo{
		public:
			void setFilters(Tiling magFilter, Tiling minFilter){
				this->magFilter = magFilter;
				this->minFilter = minFilter;
			}

			void setAddressModes(TextureAdressMode x, TextureAdressMode y){
				addressModeX = x;
				addressModeY = y;
			}

			void normalizeCoord(bool normalized){
				normalizedCoords = normalized;
			}

			void setTiling(Tiling tiling){
				this->tiling = tiling;
			}

			void setSamples(Sample samples){
				this->samples = samples;
			}

		private:
			Tiling magFilter = Tiling::Nearest;
			Tiling minFilter = Tiling::Nearest;
			TextureAdressMode addressModeX = TextureAdressMode::Repeat;
			TextureAdressMode addressModeY = TextureAdressMode::Repeat;
			bool normalizedCoords = true;
			Tiling tiling = Tiling::Nearest;
			Sample samples = Sample::Sample_1;

			// intern
			friend void convertTextureCreateInfo(TextureCreateInfo& info, void* ptr);
	};

	// ==========================================================
	// ==                         UTILS                        ==
	// ==========================================================

	/**
	 * @brief transfert the data from the void pointer into the given parameters
	 * 
	 * @param ptr the pointer to retrive the data from
	 * @param args the args where the data will be copied
	 */
	template<typename... Args>
	void convertFromVoid(void* ptr, Args&... args){
		__convertFromVoid(static_cast<char*>(ptr), args...);
	};

	// intern
	template<typename T>
	void __convertFromVoid(char* ptr, T& t){
		t = *static_cast<T*>(static_cast<void*>(ptr));
	}

	// intern
	template<typename T, typename... Args>
	void __convertFromVoid(char *ptr, T& t, Args&... args){
		t = *static_cast<T*>(static_cast<void*>(ptr));
		ptr += sizeof(T);
		__convertFromVoid(ptr, args...);
	}

	// intern
	template<typename T>
	void __convertToVoid(void* data, T& t){
		memcpy(data, &t, sizeof(T));	
	}

	// intern
	template<typename T, typename... Args>
	void __convertToVoid(void* data, T& t, Args&... args){
		memcpy(data, &t, sizeof(T));
		char* cdata = static_cast<char*>(data);
		cdata += sizeof(T);
		data = static_cast<void*>(cdata);
		__convertToVoid(data, args...);
	}

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

	/**
	 * @brief register the event into the engine's event handler
	 * 
	 * @param name the name of the event
	 * @param dataSize the size of the data carried by the event
	 * @return return the id of the event, if the event name is already used, it will return the id of the already existing event
	 */
	EventID RD_API registerEvent(const char* name, uint32_t dataSize = 0);

	/**
	 * @brief get the id of an event from it name
	 * 
	 * @param name the name of the event
	 * @return return the id of the event, if the event name is taken
	 * @throw throw an exception if the name is not taken by any event
	 */
	EventID RD_API getEventID(const char* name);

	/**
	 * @brief subscribe to an event by it's name
	 * 
	 * @param name the name of the event to subscribe to
	 * @param FNcallback a pointer to the event callback (bool foo(void* data))
	 */
	void RD_API subscribeEvent(const char* name, bool(*FNcallback)(void*));

	/**
	 * @brief subscribe to an event by it's name
	 * 
	 * @param name the name of the event to subscribe to
	 * @param instance the instance pointer of the callback methode
	 * @param MTcallback a pointer to the event callback (static bool foo(void* instance, void* data))
	 */
	void RD_API subscribeEvent(const char* name, void* instance, bool(*MTcallback)(void*, void*));
	
	/**
	 * @brief subscribe to an event by it's id
	 * 
	 * @param id the name of the event to subscribe to
	 * @param FNcallback a pointer to the event callback (bool foo(void* data))
	 */
	void RD_API subscribeEvent(EventID id, bool(*FNcallback)(void*));

	/**
	 * @brief subscribe to an event by it's id
	 * 
	 * @param id the id of the event to subscribe to
	 * @param instance the instance pointer of the callback methode
	 * @param MTcallback a pointer to the event callback (static bool foo(void* instance, void* data))
	 */
	void RD_API subscribeEvent(EventID id, void* instance, bool(*MTcallback)(void*, void*));

	/**
	 * @brief unsubscribe from an event by it's id
	 * 
	 * @param name the event to unsubscribe from
	 * @param Fncallback the pointer to the callback
	 */
	void RD_API unsubscribeEvent(const char* name, bool(*FNcallback)(void*));

	/**
	 * @brief unsubscribe from an event by it's id
	 * 
	 * @param name the event to unsubscribe from
	 * @param MTcallback the pointer to the callback
	 */
	void RD_API unsubscribeEvent(const char* name, bool(*MTcallback)(void*, void*));
	
	/**
	 * @brief unsubscribe from an event by it's id
	 * 
	 * @param id the event to unsubscribe from
	 * @param FNcallback the pointer to the callback
	 */
	void RD_API unsubscribeEvent(EventID id, bool(*FNcallback)(void*));

	/**
	 * @brief unsubscribe from an event by it's id
	 * 
	 * @param id the event to unsubscribe from
	 * @param MTcallback the pointer to the callback
	 */
	void RD_API unsubscribeEvent(EventID id, bool(*MTcallback)(void*, void*));	

	/**
	 * @brief get the size of the data carriend by an event
	 * 
	 * @param name the nme of the event
	 * @return the size of the data carried by this event
	 */
	uint32_t RD_API getEventDataSize(const char* name);

	/**
	 * @brief get the size of the data carried by an event
	 * 
	 * @param id the event id
	 * @return the size of the data carried by this event
	 */
	uint32_t RD_API getEventDataSize(EventID id);

	// intern
	void* RD_API __eventAllocStack(uint32_t size);
	
	/**
	 * @brief trigger an event and send the data as a void pointer
	 * 
	 * @param id the id of teh event
	 * @param data the data to send
	 */
	void RD_API triggerEventPtr(EventID id, void* data);

	/**
	 * @brief trigger an event from an id
	 * 
	 * @param id the id th  e event to trigger
	 * @param args the data to send
	 */
	template<typename... Args>
	void RD_API triggerEvent(EventID id, Args... args){
		void* data = __eventAllocStack(getEventDataSize(id));
		__convertToVoid(data, args...);
		triggerEventPtr(id, data);
	}

	/**
	 * @brief trigger an event from a name
	 * 
	 * @param name the name of the event to trigger
	 * @param args the data to send
	 */
	template<typename... Args>
	void RD_API triggerEvent(const char* name, Args... args){
		triggerEvent(getEventID(name), args...);
	}

	void RD_API updateEvents();

	/**
	 * @brief check if the given key is pressed on the keyboard
	 * 
	 * @param key the key
	 * @return true 
	 * @return false 
	 */
	bool RD_API isKeyDown(Key key);

	/**
	 * @brief check if the given key is not pressed on the keyboard
	 * 
	 * @param key the key
	 * @return true 
	 * @return false 
	 */
	bool RD_API isKeyUp(Key key);

	/**
	 * @brief get the current position of the mouse, relative to the top left corner of the window
	 * 	
	 * @return vec2<float> 
	 */
	vec2<float> RD_API getMousePosition();

	/**
	 * @brief get if the button is currently pressed
	 * 
	 * @param button the button to check
	 * @return true 
	 * @return false 
	 */
	bool RD_API isMouseButtonDown(MouseButton button);

	/**
	 * @brief get if the button is not currently pressed
	 * 
	 * @param button the button to check
	 * @return true 
	 * @return false 
	 */
	bool RD_API isMouseButtonUp(MouseButton button);

	// ==========================================================
	// ==                       RENDER                         ==
	// ==========================================================

	/**
	 * @brief set the clear color of the next frame
	 * @param color the color used to clear the next frame with
	 */
	void RD_API setClearColor(const Color &color);

	/**
	 * @brief begin a frame, every draw calls have to be called after this
	 * 
	 */
	void RD_API beginFrame();

	/**
	 * @brief end a frame, every draw calls have to be called before this
	 * 
	 */
	void RD_API endFrame();

	/**
	 * @brief set the data of every vertices of the scene, copy the data
	 * 
	 * @param vertices a pointer to an array of vertex
	 * @param vertexCount the count of vertices in the scene
	 */
	void RD_API setScene(void* vertices, uint32_t vertexCount);

	/**
	 * @brief set the size in bytes of a single vertex
	 * @param size the size of a single vertex
	 */
	void RD_API setSceneVertexSize(uint64_t size);

	/**
	 * @brief render the vertices of the scene
	 * 
	 */
	void RD_API renderScene();

	/**
	 * @brief set the data of a chunk of vertex, copy the data
	 * 
	 * @param offset the offset before the chunk 
	 * @param size the count of vertices in the chunk
	 * @param data an array of vertex
	 */
	void RD_API setSceneData(uint32_t offset, uint32_t count, void* data);

	/**
	 * @brief copy the four vertices into the scene buffer
	 * 
	 * @param v0 top left corner
	 * @param v1 top right corner
	 * @param v2 bottom left corner
	 * @param v3 nottom right corner
	 */
	void RD_API renderSceneQuad(void* v0, void* v1, void* v2, void* v3);

	/**
	 * @brief copy the four vertices into the general usage buffer
	 * 
	 * @param v0 top left corner
	 * @param v1 top right corner
	 * @param v2 bottom left corner
	 * @param v3 nottom right corner
	 */
	void RD_API renderGeneralUsageQuad(void* v0, void* v1, void* v2, void* v3);

	/**
	 * @brief copy the three vertices into the scene or the general usage buffer
	 * 
	 * @param v0
	 * @param v1
	 * @param v2
	 */
	void RD_API renderGeneralUsageTrigone(void* v0, void* v1, void* v2);

	/**
	 * @brief copy the two vertices into the general usage buffer
	 * 
	 * @param v0 
	 * @param v1 
	 */
	void RD_API renderGeneralUsageLine(void* v0, void* v1);

	/**
	 * @brief send a given chunk of scene vertex to the gpu
	 * 
	 * @param offset the offset before the chunk 
	 * @param count the count of vertices in chunk
	 */
	void RD_API flushSceneData(uint32_t offset, uint32_t count);

	/**
	 * @brief set the size in bytes of a single vertex
	 * @param size the size of a single vertex
	 */
	void RD_API setGeneralUsageVertexSize(uint32_t size);

	/**
	 * @brief set the data of every vertices of the general usage buffer, copy the data
	 * 
	 * @param vertices a pointer to an array of vertex
	 * @param vertexCount the count of vertices in the general usage buffer
	 */
	void RD_API setGeneralUsageVertices(void* vertices, uint32_t vertexCount);

	/**
	 * @brief set the data of a chunk of vertex, copy the data
	 * 
	 * @param offset the offset before the chunk 
	 * @param size the count of vertices in the chunk
	 * @param data an array of vertex
	 */
	void RD_API setGeneralUsageData(uint32_t offset, uint32_t count, void* data);

	/**
	 * @brief send a given chunk of general usage vertex buffer to the gpu
	 * 
	 * @param offset the offset before the chunk 
	 * @param count the count of vertices in chunk
	 */
	void RD_API flushGeneralUsageData(uint32_t offset, uint32_t count);

	/**
	 * @brief render the vertices of the general usage buffer
	 * 
	 */
	void RD_API renderGeneralUsageVertices();

	/**
	 * @brief set the topology of the general usage buffer
	 * 
	 * @param topology, quads, trigones, lines
	 */
	void RD_API setGeneralUsageTopology(Topology topology);

	/**
	 * @brief begin the render pass to the screen's framebuffer
	 * 
	 */
	void RD_API beginSwapChainRenderPass();

	/**
	 * @brief end the render pass to the screen's framebuffer
	 * 
	 */
	void RD_API endSwapChainRenderPass();

	// ================================ SHADERS

	/**
	 * @brief create a shaderid from the infos
	 * 
	 * @param info the infos used to create the shaderid
	 * @return ShaderID 
	 */
	ShaderID RD_API createShader(ShaderCreateInfo &info);

	/**
	 * @brief destroy a shaderid
	 * 
	 * @param shaderid the shaderid to destroy
	 */
	void RD_API destroyShader(ShaderID shaderid);

	/**
	 * @brief use the shaderid for the next render call
	 * 
	 * @param shaderid the shaderid to use
	 */
	void RD_API useShader(ShaderID shaderid, uint32_t *descriptorSetIndices);

	/**
	 * @brief set the push constant of the shaderid
	 * 
	 * @param shaderid 
	 * @param pushConstant the data of the push constant
	 */
	void RD_API setShaderPushConstant(ShaderID shaderid, void* pushConstant);


	// ============================= RENDER TARGETS

	/**
	 * @brief create a render target than can be used to draw content onto it
	 * 
	 * @param info the info needed to create the object
	 * @return RenderTargetID 
	 */
	RenderTargetID RD_API createRenderTarget(RenderTargetCreateInfo& info);

	/**
	 * @brief destroy and free a render target from the memory
	 * 
	 * @param renderTargetID the render target to destroy
	 */
	void RD_API destroyRenderTarget(RenderTargetID renderTargetID);

	/**
	 * @brief start to render onto a render target
	 * 
	 * @param renderTargetID the render target to render onto
	 */
	void RD_API beginRenderTarget(RenderTargetID renderTargetID);

	/**
	 * @brief stop the rendering to a render target
	 * 
	 * @param renderTargetID the render target to stop
	 */
	void RD_API endRenderTarget(RenderTargetID renderTargetID);

	/**
	 * @brief change the size of a render target
	 * 
	 * @param renderTargetID the render target to resize
	 * @param size the size of the render target (in pixels)
	 */
	void RD_API resizeRenderTarget(RenderTargetID renderTargetID, vec2<uint32_t> size);

	// ============================= DESCRIPTOR SET

	/**
	 * @brief create a descriptor set that can be used to send data to a shaderid
	 * 
	 * @param info the information needed for the descriptor set to be created
	 * @return DescriptorSetID 
	 */
	DescriptorSetID RD_API createDescriptorSet(DescriptorSetCreateInfo& info);

	/**
	 * @brief destroy a descriptor set
	 * 
	 * @param set the descriptor set to destroy
	 */
	void RD_API destroyDescriptorSet(DescriptorSetID set);

	/**
	 * @brief write to a buffer into a descriptor set
	 * 
	 * @param set the set to write into
	 * @param setIndex the index of the set
	 * @param binding the binding of the buffer
	 * @param data the data to write
	 */
	void RD_API writeToDescriptorSetBuffer(DescriptorSetID set, uint32_t setIndex, uint32_t binding, void* data);

	/**
	 * @brief set the textureid into a descriptor set
	 * 
	 * @param set the set to change
	 * @param setIndex the index of the set in the descriptor set
	 * @param binding the binding of the textureid
	 * @param textureIndex the index of the textureid to write into, if the descriptor is a textureid array
	 * @param textureid the textureid to send
	 */
	void RD_API setDescriptorSetTexture(DescriptorSetID set, uint32_t setIndex, uint32_t binding, uint32_t textureIndex, TextureID textureid);

	// ============================= TEXTUREID

	TextureID RD_API createTextureFromRenderTarget(RenderTargetID renderTargetID, uint32_t attachment, TextureCreateInfo& info);

	TextureID RD_API createTextureFromPath(const char* path, TextureCreateInfo& info);

	TextureID* RD_API createTexturesFromPaths(const char* paths[], TextureCreateInfo* infos, uint32_t textureCount);

	TextureID RD_API createTextureFromData(TextureFormat format, vec2<uint32_t> size, void* data, TextureCreateInfo& info);

	void RD_API destroyTexture(TextureID textureid);

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

	/**
	 * @brief get a pointer to the entity component
	 * 
	 * @param entity the entity to get the component from
	 * @param typeID the id of the component to get
	 * @return void* 
	 */
	void* RD_API entityGetComponent(Entity entity, uint64_t typeID);

	/**
	 * @brief register a type of component into the ECS
	 * 
	 * @param typeID the hash code id of the component type
	 * @param typeSize the size of the component
	 */
	void RD_API registerEntityComponent(uint64_t typeID, uint64_t typeSize);

	/**
	 * @brief register a type of component into the ECS
	 * 
	 * @tparam T the type of the comonent to register
	 */
	template<typename T>
	void RD_API registerEntityComponent(){
		registerEntityComponent(typeid(T).hash_code(), sizeof(T));
	}

	/**
	 * @brief register a system to be used by the ECS
	 * 
	 * @param typeID the id of the ecs
	 * @param system the pointer to the system instance
	 */
	void RD_API registerECSSystemPtr(size_t typeID, ECSSystem* system);

	/**
	 * @brief register a system to be used by the ECS
	 * 
	 * @tparam T the type of the system
	 * @tparam Args the args needed by the system to be initialized
	 * @param args the args needed by the system to be initialized
	 * @return T* the pointer to the system
	 */
	template<typename T, typename... Args>
	T* RD_API registerECSSystem(Args... args){
		T* system = new T();
		registerECSSystemPtr(typeid(T).hash_code(), system);
		system->init(args...);
		return system;
	}

	/**
	 * @brief set the signature of the system
	 * 
	 * @param typeID the id of the system
	 * @param signature the signature of the system
	 */
	void RD_API setECSSystemSignature(size_t typeID, ECSSignature& signature);

	/**
	 * @brief set the signature of a system
	 * 
	 * @tparam T the system 
	 * @param signature the signature to set to the system
	 */
	template<typename T>
	void RD_API setECSSystemSignature(ECSSignature& signature){
		setECSSystemSignature(typeid(T).hash_code(), signature);
	}

	uint32_t RD_API getComponentID(uint64_t typeID);

	template<typename T>
	uint32_t RD_API getComponentID(){
		return getComponentID(typeid(T).hash_code());
	}

	// ==========================================================
	// ==                       ASSETS                         ==
	// ==========================================================

	/**
	 * @brief register an asset factory into the engine, the factory will be called every time an asset is created and destroyed
	 * @param factory a pointer to an factory instance (AS TO BE A CHILD OF THE CLASS AssetFactory)
	 */
	void RD_API registerAssetFactoryPtr(AssetFactory *factory);

	/**
	 * @brief register an asset factory into the engine, the factory will be called every time an asset is created and destroyed
	 * 
	 * @tparam T the type of the factory
	 * @tparam Args the args for the initialization of the factory
	 * @param args 
	 */
	template<typename T, typename... Args>
	void RD_API registerAssetFactory(Args... args){
		T* factory = new T();
		factory->init(args...);
		registerAssetFactoryPtr(factory);
	}

	/**
	 * @brief get a pointer to an asset
	 * 
	 * @param typeID the id of the asset
	 * @param name the name of the asset
	 * @param data the data of the asset, will be used only to initialize the asset if it not exist
	 * @param count the count of references to this asset
	 * @param removeAsset the callback to call when the ref count goes down to zero
	 * @return void* the asset
	 */
	void* RD_API getAssetPtr(uint64_t typeID, const char* name, AssetData &data, uint16_t*& count, void (**removeAsset)(const char*));

	/**
	 * @brief get a reference to an asset
	 * 
	 * @tparam T the type of the asset
	 * @tparam Args the args needed to initialize the asset, only if the asset do not exist before this call
	 * @param name the name of the asset
	 * @param args 
	 * @return AssetReference<T> 
	 */
	template<typename T, typename... Args>
	AssetReference<T> RD_API getAsset(const char* name, Args&... args){
		AssetReference<T> ref;
		ref.asset = createAssetPtr(typeid(T).hash_code(), name, AssetData(args...), &ref.count, &ref.removeAsset);
		return ref;
	}

	/**
	 * @brief destroy all assets in the engine
	 * 
	 */
	void RD_API clearAssets();

	// ==========================================================
	// ==                         SOUNDID                        ==
	// ==========================================================

	/**
	 * @brief load a music that will then be played, the duration of the music can be long since the music is streamed instead of fully loaded into the memory
	 * 
	 * @param filepath the filepath to the music
	 */
	void RD_API loadMusic(const char* filepath);

	/**
	 * @brief pause the music
	 * 
	 */
	void RD_API pauseMusic();

	/**
	 * @brief play the music, if it is paused
	 * 
	 */
	void RD_API playMusic();

	/**
	 * @brief stop the music and delete the loaded parts
	 * 
	 */
	void RD_API stopMusic();

	/**
	 * @brief set the position of the origin of the music
	 * 
	 * @param position the position in a 3D world
	 */
	void RD_API setMusicSourcePosition(vec3<float> position);

	// =============================== soundid effects

	/**
	 * @brief load a soundid effect from disk
	 * 
	 * @param filepath the path to the soundid file
	 * @return SoundID 
	 */
	SoundID RD_API loadSoundEffect(const char* filepath);

	/**
	 * @brief destroy a soundid effect
	 * 
	 * @param soundid the soundid effect to destroy
	 */
	void RD_API destroySoundEffect(SoundID soundid);

	// ============================= soundid sources

	/**
	 * @brief create a soundid source
	 * 
	 * @return SoundSourceID 
	 */
	SoundSourceID RD_API createSoundSource();

	/**
	 * @brief destroy a soundid source
	 * 
	 * @param source the soundid source to destroy
	 */
	void RD_API destroySoundSource(SoundSourceID source);

	/**
	 * @brief set the pitch of the soundid that is coming out of this soundid source
	 * 
	 * @param source the source to modifie
	 * @param pitch the pitch of the soundid
	 */
	void RD_API setSoundSourcePitch(SoundSourceID source, float pitch);

	/**
	 * @brief set the gain of the soundid that is coming out the this soundid source
	 * 
	 * @param source the source to modifie
	 * @param gain the gain of the soundid
	 */
	void RD_API setSoundSourceGain(SoundSourceID source, float gain);

	/**
	 * @brief set the position in the world of the soundid source
	 * 
	 * @param source the source to modifie
	 * @param position the position of the soundid
	 */
	void RD_API setSoundSourcePosition(SoundSourceID source, vec3<float> position);

	/**
	 * @brief set the velocity of the soundid source
	 * 
	 * @param source the soundid to modifie
	 * @param velocity the velocity of the soundid source
	 */
	void RD_API setSoundSourceVelocity(SoundSourceID source, vec3<float> velocity);

	/**
	 * @brief enable or not the soundid source looping the soundid
	 * 
	 * @param source the source to modifie
	 * @param loop is the soundid looping or not
	 */
	void RD_API setSoundSourceLoop(SoundSourceID source, bool loop);

	/**
	 * @brief play the given soundid at the soundid source
	 * 
	 * @param source the soundid source to play from
	 * @param soundid the soundid to play
	 */
	void RD_API playSoundSource(SoundSourceID source, SoundID soundid);

	// ========================== listener

	/**
	 * @brief set the position listener, the microphone of the scene
	 * 
	 * @param position the position
	 */
	void RD_API setListenerPosition(vec3<float> position);

	/**
	 * @brief set the orientation listener, the microphone of the scene
	 * 
	 * @param at a unit vector of where the listener is looking at
	 * @param up a unit vector of where is the up relative to the listener
	 */
	void RD_API setListenerOrientation(vec3<float> at, vec3<float> up);

	/**
	 * @brief set the gain of the listener, the microphone of the scene
	 * 
	 * @param gain the gain of the listener
	 */
	void RD_API setListenerGain(float gain);

	/**
	 * @brief set the pitch of the listener, the microphone of the scene
	 * 
	 * @param pitch the pitch of the listener
	 */
	void RD_API setListenerPitch(float pitch);

	// ========================== soundid effects

	/**
	 * @brief set the effect of a soundid source
	 * 
	 * @param source the source to modifie
	 * @param reverb enable the reverberation
	 * @param lowpass enable the lowpass
	 */
	void RD_API setSoundSourceEffects(SoundSourceID source, bool reverb, bool lowpass);

	/**
	 * @brief set the effects of the music
	 * 
	 * @param reverb enable the reverberation
	 * @param lowpass enable the lowpass
	 */
	void RD_API setMusicEffects(bool reverb, bool lowpass);

	/**
	 * @brief change the reverberation type
	 * 
	 * @param type the new type of reverberation
	 */
	void RD_API setReverbType(ReverbType type);

	// ==========================================================
	// ==                TYPES WITH DEPENDENCES                ==
	// ==========================================================

	class RD_API Entity{
		public:
			Entity(const Entity &) = default;
			Entity(EntityID id) : id{id}{}

			EntityID getUID() const {return id;}

			template<typename T>
			T& addComponent(T t={}){
				entityAddComponent(id, &t, typeid(T).hash_code());
				return *static_cast<T*>(entityGetComponent(id, typeid(T).hash_code()));
			}

			template<typename T>
			void removeComponent(){
				entityRemoveComponent(id, typeid(T).hash_code());
			}

			template<typename T>
			bool hasComponent(){
				return entityHasComponent(id, typeid(T).hash_code());
			}

			template<typename T>
			T& getComponent(){
				return *static_cast<T*>(entityGetComponent(id, typeid(T).hash_code()));
			}

		private:
			EntityID id; 
	};

	class RD_API Shader{
		public:
			Shader() = default;
			Shader(const Shader &) = default;
			Shader(ShaderID id) : id{id}{}
			Shader& operator=(ShaderID id) {this->id = id; return *this;}

			void use(uint32_t* descriptorSetIndices){
				useShader(id, descriptorSetIndices);
			}

			void setPushConstant(void* data){
				setShaderPushConstant(id, data);
			}

			template<typename T>
			void setPushConstant(T &t){
				setShaderPushConstant(id, &t);
			}

			operator ShaderID() const {return id;}

		private:
			ShaderID id;
	};

	class RD_API RenderTarget{
		public:
			RenderTarget() = default;
			RenderTarget(const RenderTarget &) = default;
			RenderTarget(RenderTargetID id) : id{id}{}
			RenderTarget& operator=(RenderTargetID id) {this->id = id; return *this;}

			void begin(){
				beginRenderTarget(id);
			}

			void end(){
				endRenderTarget(id);
			}

			void resize(vec2<uint32_t> size){
				resizeRenderTarget(id, size);
			}

			operator RenderTargetID() const {return id;}
		
		private:
			RenderTargetID id;
	};

	class RD_API DescriptorSet{
		public:
			DescriptorSet() = default;
			DescriptorSet(const DescriptorSet &) = default;
			DescriptorSet(DescriptorSetID id) : id{id}{}
			DescriptorSet& operator=(DescriptorSetID id) {this->id = id; return *this;}

			void writeToBuffer(uint32_t setIndex, uint32_t binding, void* data){
				writeToDescriptorSetBuffer(id, setIndex, binding, data);
			}

			template<typename T>
			void writeToBuffer(T& data, uint32_t setIndex, uint32_t binding){
				writeToDescriptorSetBuffer(id, setIndex, binding, &data);
			}

			void setTexture(uint32_t setIndex, uint32_t binding, uint32_t textureIndex, TextureID textureID){
				setDescriptorSetTexture(id, setIndex, binding, textureIndex, textureID);
			}

			operator DescriptorSetID() const {return id;}
			DescriptorSetID* operator&() {return &id;}

		private:
			DescriptorSetID id;
	};

	class RD_API Texture{
		public:
			Texture() = default;
			Texture(const Texture &) = default;
			Texture(TextureID id) : id{id}{}
			Texture& operator=(TextureID id) {this->id = id; return *this;}
		
			operator TextureID() const {return id;}
			TextureID* operator&() {return &id;}
		private:
			TextureID id;
	};

	class RD_API Sound{
		public:
			Sound() = default;
			Sound(const Sound &) = default;
			Sound(SoundID id) : id{id}{}
			Sound& operator=(SoundID id) {this->id = id; return *this;}
			
			operator SoundID() const {return id;}

		private:
			SoundID id;
	};

	class RD_API SoundSource{
		public:
			SoundSource() = default;
			SoundSource(const SoundSource &) = default;
			SoundSource(SoundSourceID id) : id{id}{}
			SoundSource& operator=(SoundSourceID id) {this->id = id; return *this;}

			void setPitch(float pitch){
				setSoundSourcePitch(id, pitch);
			}

			void setGain(float gain){
				setSoundSourceGain(id, gain);
			}

			void setPosition(vec3<float> position){
				setSoundSourcePosition(id, position);
			}

			void setVelocity(vec3<float> velocity){
				setSoundSourceVelocity(id, velocity);
			}

			void setLoop(bool loop){
				setSoundSourceLoop(id, loop);
			}

			void play(SoundID sound){
				playSoundSource(id, sound);
			}

		private:
			SoundSourceID id;
	};

	template<typename T>
	using Ref = AssetReference<T>;
}

#undef RD_MAT
#endif // _RAINDROP_HPP_