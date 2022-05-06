#pragma once

#include "../core.hpp"
#include "Scene.hpp"

namespace engine{
	class ENGINE_API SceneSerializer{
		public:
			SceneSerializer(const Ref<Scene> &scene, const Ref<Texture2DLibrary> &texturesLibrary);
			static Ref<SceneSerializer> create(const Ref<Scene> &scene, const Ref<Texture2DLibrary> &texturesLibrary);

			void serializeText(const std::string &filepath);
			void serializeBin(const std::string &filepath);

			bool deserializeText(const std::string &filepath);
			bool deserializeBin(const std::string &filepath);
		
		private:
			void deserializeSpriteQueue(YAML::Node data);

			Ref<Scene> scene;
			Ref<Texture2DLibrary> texturesLibrary;
	};
}