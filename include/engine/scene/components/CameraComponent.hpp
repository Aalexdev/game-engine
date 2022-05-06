#pragma once

#include "../../core.hpp"
#include "../SceneCamera.hpp"

namespace engine::components{
	struct ENGINE_API Camera{
		uint32_t viewportWidth, viewportHeight;

		// if false, will get the viewport's aspect ratio
		bool fixedViewportAspectRatio = false;

		// the camera with the most higher value will be the main camera
		float priorityLevel = 1.f;

		engine::SceneCamera camera;
	};
}