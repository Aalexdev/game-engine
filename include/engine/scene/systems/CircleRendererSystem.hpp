#pragma once

#include "../../core.hpp"
#include "../ECS.hpp"
#include "../Entity.hpp"
#include "../components/CircleRendererComponent.hpp"

namespace engine{
	class Scene;
}

namespace engine::ECS::systems{
	class CircleRenderer : public System{
		public:
			CircleRenderer(Scene *scene);
			~CircleRenderer() = default;

			virtual void init() override;
			std::unordered_map<uint16_t, std::list<engine::Entity>> sort(std::set<uint16_t> &ids);
		
		private:
			Scene *scene;
	};
}