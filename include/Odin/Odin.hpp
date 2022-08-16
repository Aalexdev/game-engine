#pragma once

#include <iostream>
#include <unordered_map>
#include <cassert>
#include <pthread.h>

#include "../horreum/Horreum.hpp"

#ifndef NDEBUG
	#include <cstring>
#endif

class Odin{
	private:	
		struct AssetData{
			void* assetPtr = nullptr;
			uint16_t refCount = 0;
			size_t hashCode = 0;

			#ifndef NDEBUG
				char* assetType = nullptr;
				char* name = nullptr;
			#endif
		};

	public:
		class Data{
			public:
				Data(){}
				~Data(){
					if (data) HRM_FREE(data);
				}

				template<typename... Args>
				Data(Args&&... args){
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
					data = HRM_MALLOC(size);
					setData(size, args...);
				}

				void reset(){
					if (data) HRM_FREE(data);
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

		class Factory{
			public:
				Factory() = default;
				virtual ~Factory() = default;

				virtual size_t getTypeHash() const = 0;
				virtual void* create(const Data &data) = 0;
				virtual void destroy(void* asset) = 0;
		};

		template<typename T>
		class Reference{
			friend class Odin;

			using IT = std::unordered_map<std::string, AssetData>::iterator;
			public:
				Reference(){}
				Reference(const Reference<T> &other){
					set(other.it);
				}

				Reference(const IT &it){
					set(it);
				}

				~Reference(){reset();}
				void reset() {if (*this){removeRef();}}

				uint16_t refCount() const {if (*this){return it->second.refCount;} return 0;}

				void operator=(const Reference<T>& other){
					set(other.it);
				}

				T* operator->() const {return &it->second;}
				T& operator*() {return *static_cast<T*>(it->second.assetPtr);}
				operator bool() const {return it != getInstance().nameToAssetMap.end();}
				bool operator==(const Reference<T> &other) {return it == other.it;}
				bool operator!=(const Reference<T> &other) {return it != other.it;}
				
			private:
				void set(IT it){
					this->it = it;
					it->second.refCount++;
				}

				void removeRef(){
					if (!*this) return;

					auto& ref = it->second.refCount;
					ref--;
					if (ref == 0){
						removeAsset(it);
					}
				}

				IT it;
		};

		Odin();
		~Odin();

		static void initialize();

		template<typename T, typename... Args>
		static void registerFactory(Args... args){
			Odin& instance = getInstance();
			T* factory = new T();
			factory->init(args...);
			instance.typeToFactoryMap[factory->getTypeHash()] = factory;
		}

		/**
		 * @brief return a reference to the requested asset, load the asset if it was not loaded before
		 * 
		 * @tparam T 
		 * @param filepath the path to the asset to load, an absloute path or a path relative to the .data folder
		 * @return Reference<T> 
		 */
		template<typename T, typename... Args>
		static Reference<T> getAsset(const char* name, Args&&... args){
			Odin& instance = getInstance();
			// check if the asset as already been loaded

			pthread_mutex_lock(&instance.lock);
			auto it = instance.nameToAssetMap.find(name);
			bool assetDontExist = it == instance.nameToAssetMap.end();

			if (assetDontExist){
				auto ref = loadAsset<T, Args...>(name, args...);
				pthread_mutex_unlock(&instance.lock);
				return ref;
			}

			Reference<T> ref(it);
			
			pthread_mutex_unlock(&instance.lock);
			return ref;
		}
		
		template<typename T, typename... Args>
		static void reload(const char* name, Args&&... args){
			Odin& instance = getInstance();

			pthread_mutex_lock(&instance.lock);
			auto it = instance.nameToAssetMap.find(name);

			removeAsset(it);
			loadAsset<T, Args...>(name, args...);
			pthread_mutex_unlock(&instance.lock);
		}

		static void clear();
		
	private:
		static Odin& getInstance();

		template<typename T>
		static Factory* getTypeFactory(){
			return getTypeFactory(typeid(T).hash_code());
		}

		static Factory* getTypeFactory(size_t hashCode){
			Odin& instance = getInstance();

			auto it = instance.typeToFactoryMap.find(hashCode);
			assert(it != instance.typeToFactoryMap.end() && "factory not registred for this type of asset");

			return it->second;
		}

		// this function should be call when the asset is not loaded into the asset manager, if it is, it will be overwrite
		template<typename T, typename... Args>
		static Reference<T> loadAsset(const char *name, Args&... args){
			Odin &instance = getInstance();
			Factory* factory = getTypeFactory<T>();

			void* asset = factory->create(Data(args...));
			assert(asset != nullptr && "cannot use a nullptr as an asset");

			AssetData data;
			data.assetPtr = asset;
			data.hashCode = typeid(T).hash_code();
			data.refCount = 0;
			
			#ifndef NDEBUG
				data.assetType = static_cast<char*>(HRM_MALLOC(sizeof(char) * strlen(typeid(T).name())));
				strcpy(data.assetType, typeid(T).name());

				data.name = static_cast<char*>(HRM_MALLOC(sizeof(char) * strlen(name)));
				strcpy(data.name, name);
			#endif

			auto it = instance.nameToAssetMap.find(name);
			bool assetExist = it != instance.nameToAssetMap.end();

			// if the asset name is already used, we delete the previous asset
			if (assetExist){
				removeAsset(it);
			}

			instance.nameToAssetMap.insert({name, data});
			it = instance.nameToAssetMap.find(name);

			Reference<T> ref(it);
			return ref;
		}

		static void removeAsset(std::unordered_map<std::string, AssetData>::iterator it);
		static void deleteAssetData(AssetData &asset);

		pthread_mutex_t lock;
		std::unordered_map<size_t, Factory*> typeToFactoryMap;
		std::unordered_map<std::string, AssetData> nameToAssetMap;
};