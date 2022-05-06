#include "engine/pch.hpp"
#include "engine/renderer/Texture2DLibrary.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/filesystem/Filesystem.hpp"

namespace engine{
	void Texture2DLibrary::push(const Ref<Texture2D> &texture){
		ENGINE_PROFILE_FUNCTION();
		textures[texture->getUUID()] = texture;
		texturesList.push_back(texture);
		paths[texture->getPath()] = texture->getUUID();
	}

	void Texture2DLibrary::pop(const Ref<Texture2D> &texture){
		ENGINE_PROFILE_FUNCTION();
		paths.erase(texture->getPath());
		textures.erase(texture->getUUID());
		texturesList.remove(texture);
	}

	void Texture2DLibrary::pop(UUID id){
		ENGINE_PROFILE_FUNCTION();
		paths.erase(textures[id]->getPath());
		texturesList.remove(textures[id]);
		textures.erase(id);
	}

	Ref<Texture2D> Texture2DLibrary::load(const std::filesystem::path &path){
		ENGINE_PROFILE_FUNCTION();

		std::filesystem::path rel = std::filesystem::relative(path, Filesystem::getDataFolderPath());
		if (paths.find(rel) != paths.end()){
			return textures[paths[rel]];
		}

		Ref<Texture2D> texture = Texture2D::create(path.string());
		ENGINE_CORE_ASSERT(texture, "failed to load the texture");
		push(texture);
		return texture;
	}

	Ref<Texture2D> Texture2DLibrary::load(const std::filesystem::path &path, const std::string &name, UUID uuid){
		ENGINE_PROFILE_FUNCTION();

		std::filesystem::path rel = std::filesystem::relative(path, Filesystem::getDataFolderPath());
		if (paths.find(rel) != paths.end()){
			return textures[paths[rel]];
		}

		Ref<Texture2D> texture = Texture2D::create(path.string(), Ref<Texture2DLibrary>(this));
		ENGINE_CORE_ASSERT(texture, "failed to load the texture");

		texture->setName(name);
		texture->setUUID(uuid);
		
		push(texture);
		return texture;
	}


	bool Texture2DLibrary::exist(const Ref<Texture2D> &texture){
		return textures.find(texture->getUUID()) != textures.end();
	}

	bool Texture2DLibrary::exist(UUID id){
		return textures.find(id) != textures.end();
	}

	Ref<Texture2D> Texture2DLibrary::get(UUID id){
		auto it = textures.find(id);
		ENGINE_CORE_ASSERT(it != textures.end(), "a texture with the given id do not exist or is not registred");

		return it->second;
	}
}