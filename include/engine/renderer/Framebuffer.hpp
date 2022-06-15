#pragma once

#include "../core.hpp"
#include "FramebufferLayout.hpp"
#include "Renderer.hpp"

namespace engine{
	class ENGINE_API Framebuffer{
		public:
			virtual ~Framebuffer() {}
			
			static Ref<Framebuffer> create(Renderer *renderer, const FramebufferLayout &layout);

			virtual void bind() = 0;
			virtual void unBind() = 0;

			virtual void rebuild() = 0;
			virtual void resize(uint32_t width, uint32_t height) = 0;

			// return the color attachment at the given slot index, if non existent, return 0
			virtual uint32_t getColorAttachment(uint32_t slot) const = 0;
			virtual uint32_t getDepthAttachment() const = 0;

			virtual void getPixel(uint32_t attachment, uint32_t x, uint32_t y, void* pixel) = 0;
			virtual void clear(uint32_t attachment, void* clearData) = 0;

			virtual uint32_t getWidth() const = 0;
			virtual uint32_t getHeight() const = 0;

			virtual bool bound() const = 0;

			float getAspectRatio() const{
				return static_cast<float>(getWidth()) / static_cast<float>(getHeight());
			}
	};
}