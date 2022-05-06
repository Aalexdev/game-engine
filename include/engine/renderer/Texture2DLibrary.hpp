#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "../UUID.hpp"
#include "Texture2D.hpp"

namespace engine{
	class ENGINE_API Texture2DLibrary{
		public:
			Texture2DLibrary() = default;
			~Texture2DLibrary() = default;

			void push(const Ref<Texture2D> &texture);

			void pop(const Ref<Texture2D> &texture);
			void pop(UUID id);

			Ref<Texture2D> load(const std::filesystem::path &path);
			Ref<Texture2D> load(const std::filesystem::path &path, const std::string &name, UUID uuid);

			bool exist(const Ref<Texture2D> &texture);
			bool exist(UUID id);

			Ref<Texture2D> get(UUID id);

			size_t size() const {return textures.size();}

			std::unordered_map<UUID, Ref<Texture2D>> &getMap() {return textures;}
			std::list<Ref<Texture2D>> &getList() {return texturesList;}
			std::unordered_map<std::filesystem::path, UUID> &getPats() {return paths;}

		private:
			std::list<Ref<Texture2D>> texturesList;

			std::unordered_map<UUID, Ref<Texture2D>> textures;
			std::unordered_map<std::filesystem::path, UUID> paths;
	};
}