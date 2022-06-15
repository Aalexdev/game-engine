#pragma once

#include "../core.hpp"
#include "../UUID.hpp"

namespace engine{

	enum class TextureFormat{
		NONE = 0,
		R    = 1,
		RG   = 2,
		RGB  = 3,
		RGBA = 4
	};

	class Renderer;

	class ENGINE_API Texture{
		public:
			virtual ~Texture() {}

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;
			virtual TextureFormat getFormat() const = 0;

			virtual void bind(uint32_t slot = 0) const = 0;
			virtual void setData(void *data, uint32_t size, uint32_t offset = 0) = 0;
			virtual uint32_t getTexture() const = 0;

			virtual bool operator==(const Texture &texture) const = 0;

			virtual Renderer* getRenderer() const = 0;

	};
}