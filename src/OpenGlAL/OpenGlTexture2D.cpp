#include "engine/core.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/filesystem/Filesystem.hpp"
#include "engine/OpenGlAL/OpenGlTexture2D.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <libs/glad/glad.h>
#include <libs/stb/stb_image.h>

namespace engine::OpenGl{
	static TextureFormat stbFormatToEngineFormat(int channels){
		switch (channels){
			case STBI_grey: return TextureFormat::R;
			case STBI_grey_alpha: return TextureFormat::RG;
			case STBI_rgb: return TextureFormat::RGB;
			case STBI_rgb_alpha: return TextureFormat::RGBA;
		}
		ENGINE_CORE_ASSERT(false, "unknown texture format");
		return TextureFormat::NONE;
	};

	static GLenum formatToGLInternalFormat(TextureFormat format){
		switch (format){
			case TextureFormat::R:     return GL_R8;
			case TextureFormat::RG:    return GL_RG8;
			case TextureFormat::RGB:   return GL_RGB8;
			case TextureFormat::RGBA:  return GL_RGBA8;
		}
		ENGINE_CORE_ASSERT(false, "unknwon TextureFormat");
		return 0;
	}

	static GLenum formatToGLDataFormat(TextureFormat format){
		switch (format){
			case TextureFormat::R:     return GL_RED;
			case TextureFormat::RG:    return GL_RG;
			case TextureFormat::RGB:   return GL_RGB;
			case TextureFormat::RGBA:  return GL_RGBA;
		}
		ENGINE_CORE_ASSERT(false, "unknwon TextureFormat");
		return 0;
	}

	OpenGlTexture2D::OpenGlTexture2D(const std::string &filepath, const Ref<Texture2DLibrary> &library) : library{library}{
		ENGINE_PROFILE_FUNCTION();

		std::filesystem::path path(filepath);
		name = path.filename().string();
		this->path = std::filesystem::relative(path, Filesystem::getDataFolderPath());

		int width, height, channels;
		stbi_uc* pixels = nullptr;
		
		stbi_set_flip_vertically_on_load(true);

		{
			ENGINE_PROFILE_SCOPE("load texture file");
			pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}

		ENGINE_CORE_ASSERT(pixels, "failed to load \"", filepath, "\"");

		this->width = width;
		this->height = height;
		this->format = stbFormatToEngineFormat(channels);

		ENGINE_CORE_ASSERT(format != TextureFormat::NONE, "invalid texture format");
		
		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = formatToGLInternalFormat(format);
		dataFormat = formatToGLDataFormat(format);

		{
			ENGINE_PROFILE_SCOPE("glTexture creation and parameters setup");
			glCreateTextures(GL_TEXTURE_2D, 1, &texture);
			glTextureStorage2D(texture, 1, internalFormat, this->width, this->height);
			glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		setData(pixels, width * height);
		stbi_image_free(pixels);

		if (library){
			library->push(Ref<Texture2D>(this));
		}
	}

	OpenGlTexture2D::OpenGlTexture2D(uint32_t width, uint32_t height, TextureFormat format, void *data, const Ref<Texture2DLibrary> &library) : format{format}, width{width}, height{height}, library{library}{
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_ASSERT(format != TextureFormat::NONE, "TextureFormat::NONE not supported");
		GLenum internalFormat = formatToGLInternalFormat(format);

		ENGINE_CORE_ASSERT(internalFormat, "unknown channel format");

		{
			ENGINE_PROFILE_SCOPE("gl texture creation and parameters setup");
			glCreateTextures(GL_TEXTURE_2D, 1, &texture);
			glTextureStorage2D(texture, 1, internalFormat, this->width, this->height);
			glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		if (data) setData(data, width * height);

		if (library){
			library->push(Ref<Texture2D>(this));
		}
	}

	OpenGlTexture2D::~OpenGlTexture2D(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteTextures(1, &texture);
	}

	void OpenGlTexture2D::bind(uint32_t slot) const{
		ENGINE_PROFILE_FUNCTION();
		glBindTextureUnit(slot, texture);
	}

	void OpenGlTexture2D::setData(void *data, uint32_t size, uint32_t offset){
		ENGINE_PROFILE_FUNCTION();
		glTextureSubImage2D(texture, 0, 0, 0, width, height, formatToGLDataFormat(format), GL_UNSIGNED_BYTE, data);
	}

}