#pragma once

#include "../core.hpp"
#include "../renderer/Texture2D.hpp"
#include "../renderer/Renderer.hpp"

namespace engine::OpenGL{
	class ENGINE_API Texture2D : public engine::Texture2D{
		public:
			static Ref<Texture2D> create(Renderer *renderer, uint32_t width, uint32_t height, TextureFormat format, void *data);

			Texture2D(Renderer *renderer, uint32_t width, uint32_t height, TextureFormat format, void *data);
			~Texture2D();

			virtual uint32_t getWidth() const override {return width;}
			virtual uint32_t getHeight() const override {return height;}
			virtual TextureFormat getFormat() const override {return format;}

			virtual void bind(uint32_t slot = 0) const override;
			virtual void setData(void *data, uint32_t size, uint32_t offset = 0) override;
			virtual bool operator==(const Texture &texture) const override {return this->texture == static_cast<const Texture2D&>(texture).texture;}

			virtual uint32_t getTexture() const override {return texture;}
			virtual Renderer* getRenderer() const override;

		private:
		 	Renderer *renderer;
			uint32_t width;
			uint32_t height;
			TextureFormat format;

			uint32_t texture;
	};
}