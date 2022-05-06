#pragma once

#include "../core.hpp"
#include "Texture.hpp"
#include <libs/glm/glm.hpp>

namespace std{
	template<>
	struct hash<filesystem::path>{
		size_t operator()(const filesystem::path &path) const{
			return std::filesystem::hash_value(path);
		}
	};
}


namespace engine{

	class Texture2DLibrary;

	class ENGINE_API Texture2D : public Texture{
		public:
			static Ref<Texture2D> create(const std::string &filepath, const Ref<Texture2DLibrary> &library = nullptr);
			static Ref<Texture2D> create(uint32_t width, uint32_t height, TextureFormat format = TextureFormat::RGBA, void* data = nullptr, const Ref<Texture2DLibrary> &library = nullptr);

			
			const UUID& getUUID() const {return uuid;}
			void setUUID(const UUID& uuid) {this->uuid = uuid;}

			const std::string& getName() const {return name;}
			void setName(const std::string& name) {this->name = name;}

			const std::filesystem::path &getPath() const {return path;}

			virtual Texture2DLibrary* getLibrary() const = 0;

			static glm::vec2 topCorner();
			static glm::vec2 bottomCorner();
		
		protected:

			// used to reconize the texture in serialized scene
			UUID uuid;

			// obsional, is used as user-only to reconize the texture
			std::string name;

			std::filesystem::path path;
	};
}