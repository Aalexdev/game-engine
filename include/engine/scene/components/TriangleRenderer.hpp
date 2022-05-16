#pragma once

#include "../../core.hpp"
#include "../../renderer/Renderer.hpp"

namespace engine::ECS::components{
	struct ENGINE_API TriangleRenderer{
		Renderer::Vertex p1;
		Renderer::Vertex p2;
		Renderer::Vertex p3;

		uint16_t batchGroup = 0;
		
		Ref<Texture2D> texture;

		TriangleRenderer(const TriangleRenderer &) = default;
		TriangleRenderer() = default;
	};
}