#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"
#include "../Entity.hpp"

namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class TriangleRenderer : public System{
		public:
			TriangleRenderer(Scene *scene) : scene{scene}{}
			~TriangleRenderer() = default;

			virtual void init() override;
			std::unordered_map<uint16_t, std::list<engine::Entity>> sort(std::set<uint16_t> &ids);
		
		private:
			Scene *scene;

	};
}