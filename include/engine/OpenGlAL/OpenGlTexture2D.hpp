#pragma once

#include "../core.hpp"
#include "../renderer/Texture2D.hpp"
#include "../renderer/Texture2DLibrary.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlTexture2D : public Texture2D{
		public:
			OpenGlTexture2D(const std::string &filepath, const Ref<Texture2DLibrary>& library);
			OpenGlTexture2D(uint32_t width, uint32_t height, TextureFormat format, void *data, const Ref<Texture2DLibrary>& library);
			~OpenGlTexture2D();

			virtual uint32_t getWidth() const override {return width;}
			virtual uint32_t getHeight() const override {return height;}
			virtual TextureFormat getFormat() const override {return format;}

			virtual void bind(uint32_t slot = 0) const override;
			virtual void setData(void *data, uint32_t size, uint32_t offset = 0) override;
			virtual bool operator==(const Texture &texture) const override {return this->texture == static_cast<const OpenGlTexture2D&>(texture).texture;}

			virtual uint32_t getTexture() const override {return texture;}

			virtual Texture2DLibrary* getLibrary() const {return library.get();}

		private:
			uint32_t width;
			uint32_t height;
			TextureFormat format;

			uint32_t texture;
			Ref<Texture2DLibrary> library;
	};
}