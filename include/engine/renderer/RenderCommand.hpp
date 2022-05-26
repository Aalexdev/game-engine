#pragma once

#include "../core.hpp"
#include "RendererAPI.hpp"

namespace engine{
	struct ENGINE_API RenderCommand{

		enum Type{
			TYPE_NONE = 0,
			TYPE_QUAD, // [Data]
			TYPE_TEXTURED_QUAD, // [Data, texture data]
			TYPE_CLEAR, // [null]
			TYPE_SET_CLEAR_COLOR, // [Data]
		};
		
		enum DataSpecs{
			DATA_NONE,
			DATA_TRANSFORM, // [transform mat4]
			DATA_TRANSLATE, // [X, y]
			DATA_TRANSLATE_SCALE1, // [X, Y, scaleXY]
			DATA_TRANSLATE_SCALE2, // [X, Y, scaleX, scaleY]
			DATA_TRANSLATE_SCALE1_ROTATE, // [X, Y, scaleXY, rotation]
			DATA_TRANSLATE_SCALE2_ROTATE, // [X, Y, scaleX, scaleY, rotation]
			DATA_TRANSLATE_ROTATE, // [X, Y, rotation]
		};

		enum ColorSpec{
			COLOR_NONE,
			COLOR_WHITE,
			COLOR_RGB,
			COLOR_RGBA
		};

		Type type = TYPE_NONE;
		DataSpecs specs = DATA_NONE;
		ColorSpec colorSpecs = COLOR_NONE;

		float* data = nullptr;
		uint16_t count = 1;
	};
}