#pragma once

#include "../core.hpp"

namespace engine{
	struct ENGINE_API RenderCommand{
		enum class Category{
			NONE = 0,

			// draw
			DRAW_QUAD,
			DRAW_QUAD_COLORED,
			DRAW_TEXTURED_QUAD,
			DRAW_TEXTURED_QUAD_COLORED,

			// renderer properties
			SET_REFRESH_RATE,
			SET_CLEAR_COLOR,
			SET_VIEWPORT,
			SET_RESOLUTION,
			SET_VIEW_PROJECTION_MAT,
			RESIZED,
			ENABLE_VSYNC,
			CLEAR,
			USE_SHADER,
		};

		Category category = Category::NONE;
		float *data = nullptr;
	};
}