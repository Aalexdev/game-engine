#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGlAL/OpenGlFramebuffer.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGl{

	static int getGLColorAttachmentIndex(int index){
		return GL_COLOR_ATTACHMENT0 + index;
	}

	static GLint getInternalFormat(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::R8: return GL_R8;
			case FramebufferTextureFormat::RG8: return GL_RG8;
			case FramebufferTextureFormat::RGB8: return GL_RGB8;
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::R16F: return GL_R16F;
			case FramebufferTextureFormat::RG16F: return GL_RG16F;
			case FramebufferTextureFormat::RGB16F: return GL_RGB16F;
			case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
			case FramebufferTextureFormat::R8I: return GL_R8I;
			case FramebufferTextureFormat::RG8I: return GL_RG8I;
			case FramebufferTextureFormat::RGB8I: return GL_RGB8I;
			case FramebufferTextureFormat::RGBA8I: return GL_RGBA8I;
			case FramebufferTextureFormat::R16: return GL_R16;
			case FramebufferTextureFormat::RG16: return GL_RG16;
			case FramebufferTextureFormat::RGB16: return GL_RGB16;
			case FramebufferTextureFormat::RGBA16: return GL_RGBA16;
			case FramebufferTextureFormat::R32I: return GL_R32I;
			case FramebufferTextureFormat::RG32I: return GL_RG32I;
			case FramebufferTextureFormat::RGB32I: return GL_RGB32I;
			case FramebufferTextureFormat::RGBA32I: return GL_RGBA32I;
			case FramebufferTextureFormat::R32F: return GL_R32F;
			case FramebufferTextureFormat::RG32F: return GL_RG32F;
			case FramebufferTextureFormat::RGB32F: return GL_RGB32F;
			case FramebufferTextureFormat::RGBA32F: return GL_RGBA32F;
			case FramebufferTextureFormat::R8UI: return GL_R8UI;
			case FramebufferTextureFormat::RG8UI: return GL_RG8UI;
			case FramebufferTextureFormat::RGB8UI: return GL_RGB8UI;
			case FramebufferTextureFormat::RGBA8UI: return GL_RGBA8UI;
			case FramebufferTextureFormat::R16UI: return GL_R16UI;
			case FramebufferTextureFormat::RG16UI: return GL_RG16UI;
			case FramebufferTextureFormat::RGB16UI: return GL_RGB16UI;
			case FramebufferTextureFormat::RGBA16UI: return GL_RGBA16UI;
			case FramebufferTextureFormat::R32UI: return GL_R32UI;
			case FramebufferTextureFormat::RG32UI: return GL_RG32UI;
			case FramebufferTextureFormat::RGB32UI: return GL_RGB32UI;
			case FramebufferTextureFormat::RGBA32UI: return GL_RGBA32UI;
			case FramebufferTextureFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
			case FramebufferTextureFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;
			case FramebufferTextureFormat::DEPTH16: return GL_DEPTH_COMPONENT16;
			case FramebufferTextureFormat::DEPTH24: return GL_DEPTH_COMPONENT24;
			case FramebufferTextureFormat::DEPTH32: return GL_DEPTH_COMPONENT32;
			case FramebufferTextureFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
		}

		ENGINE_CORE_ASSERT(false, "unknown framebuffer texture format");
		return GL_NONE;
	}

	static GLenum getFormat(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::R8: return GL_RED;
			case FramebufferTextureFormat::RG8: return GL_RG;
			case FramebufferTextureFormat::RGB8: return GL_RGB;
			case FramebufferTextureFormat::RGBA8: return GL_RGBA;
			case FramebufferTextureFormat::R16F: return GL_RED;
			case FramebufferTextureFormat::RG16F: return GL_RG;
			case FramebufferTextureFormat::RGB16F: return GL_RGB;
			case FramebufferTextureFormat::RGBA16F: return GL_RGBA;
			case FramebufferTextureFormat::R8I: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RG8I: return GL_RG_INTEGER;
			case FramebufferTextureFormat::RGB8I: return GL_RGB_INTEGER;
			case FramebufferTextureFormat::RGBA8I: return GL_RGBA_INTEGER;
			case FramebufferTextureFormat::R16: return GL_RED;
			case FramebufferTextureFormat::RG16: return GL_RG;
			case FramebufferTextureFormat::RGB16: return GL_RGB;
			case FramebufferTextureFormat::RGBA16: return GL_RGBA;
			case FramebufferTextureFormat::R32I: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RG32I: return GL_RG_INTEGER;
			case FramebufferTextureFormat::RGB32I: return GL_RGB_INTEGER;
			case FramebufferTextureFormat::RGBA32I: return GL_RGBA_INTEGER;
			case FramebufferTextureFormat::R32F: return GL_RED;
			case FramebufferTextureFormat::RG32F: return GL_RG;
			case FramebufferTextureFormat::RGB32F: return GL_RGB;
			case FramebufferTextureFormat::RGBA32F: return GL_RGBA;
			case FramebufferTextureFormat::R8UI: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RG8UI: return GL_RG_INTEGER;
			case FramebufferTextureFormat::RGB8UI: return GL_RGB_INTEGER;
			case FramebufferTextureFormat::RGBA8UI: return GL_RGBA_INTEGER;
			case FramebufferTextureFormat::R16UI: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RG16UI: return GL_RG_INTEGER;
			case FramebufferTextureFormat::RGB16UI: return GL_RGB_INTEGER;
			case FramebufferTextureFormat::RGBA16UI: return GL_RGBA_INTEGER;
			case FramebufferTextureFormat::R32UI: return GL_RED_INTEGER;
			case FramebufferTextureFormat::RG32UI: return GL_RG_INTEGER;
			case FramebufferTextureFormat::RGB32UI: return GL_RGB_INTEGER;
			case FramebufferTextureFormat::RGBA32UI: return GL_RGBA_INTEGER;
		}
		
		ENGINE_CORE_ASSERT(false, "unknown framebuffer texture format");
		return GL_NONE;
	}

	static GLenum getDataType(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::R8: return GL_FLOAT;
			case FramebufferTextureFormat::RG8: return GL_FLOAT_VEC2;
			case FramebufferTextureFormat::RGB8: return GL_FLOAT_VEC3;
			case FramebufferTextureFormat::RGBA8: return GL_FLOAT_VEC4;
			case FramebufferTextureFormat::R16F: return GL_FLOAT;
			case FramebufferTextureFormat::RG16F: return GL_FLOAT_VEC2;
			case FramebufferTextureFormat::RGB16F: return GL_FLOAT_VEC3;
			case FramebufferTextureFormat::RGBA16F: return GL_FLOAT_VEC4;
			case FramebufferTextureFormat::R8I: return GL_UNSIGNED_INT;
			case FramebufferTextureFormat::RG8I: return GL_UNSIGNED_INT_VEC2;
			case FramebufferTextureFormat::RGB8I: return GL_UNSIGNED_INT_VEC3;
			case FramebufferTextureFormat::RGBA8I: return GL_UNSIGNED_INT_VEC4;
			case FramebufferTextureFormat::R16: return GL_FLOAT;
			case FramebufferTextureFormat::RG16: return GL_FLOAT_VEC2;
			case FramebufferTextureFormat::RGB16: return GL_FLOAT_VEC3;
			case FramebufferTextureFormat::RGBA16: return GL_FLOAT_VEC4;
			case FramebufferTextureFormat::R32I: return GL_INT;
			case FramebufferTextureFormat::RG32I: return GL_INT_VEC2;
			case FramebufferTextureFormat::RGB32I: return GL_INT_VEC3;
			case FramebufferTextureFormat::RGBA32I: return GL_INT_VEC4;
			case FramebufferTextureFormat::R32F: return GL_FLOAT;
			case FramebufferTextureFormat::RG32F: return GL_FLOAT_VEC2;
			case FramebufferTextureFormat::RGB32F: return GL_FLOAT_VEC3;
			case FramebufferTextureFormat::RGBA32F: return GL_FLOAT_VEC4;
			case FramebufferTextureFormat::R8UI: return GL_UNSIGNED_INT;
			case FramebufferTextureFormat::RG8UI: return GL_UNSIGNED_INT_VEC2;
			case FramebufferTextureFormat::RGB8UI: return GL_UNSIGNED_INT_VEC3;
			case FramebufferTextureFormat::RGBA8UI: return GL_UNSIGNED_INT_VEC4;
			case FramebufferTextureFormat::R16UI: return GL_UNSIGNED_INT;
			case FramebufferTextureFormat::RG16UI: return GL_UNSIGNED_INT_VEC2;
			case FramebufferTextureFormat::RGB16UI: return GL_UNSIGNED_INT_VEC3;
			case FramebufferTextureFormat::RGBA16UI: return GL_UNSIGNED_INT_VEC4;
			case FramebufferTextureFormat::R32UI: return GL_UNSIGNED_INT;
			case FramebufferTextureFormat::RG32UI: return GL_UNSIGNED_INT_VEC2;
			case FramebufferTextureFormat::RGB32UI: return GL_UNSIGNED_INT_VEC3;
			case FramebufferTextureFormat::RGBA32UI: return GL_UNSIGNED_INT_VEC4;
			case FramebufferTextureFormat::DEPTH16: return GL_FLOAT;
			case FramebufferTextureFormat::DEPTH24: return GL_FLOAT;
			case FramebufferTextureFormat::DEPTH32: return GL_FLOAT;
			case FramebufferTextureFormat::DEPTH32F: return GL_FLOAT;
		}
		ENGINE_CORE_ASSERT(false, "unknown framebuffer texture format");
		return GL_NONE;
	}

	static GLint getFiltering(FramebufferTextureFiltering filter){
		switch (filter){
			case FramebufferTextureFiltering::LINEAR: return GL_LINEAR;
			case FramebufferTextureFiltering::NEAREST: return GL_NEAREST;
		}
		ENGINE_CORE_ASSERT(false, "unknown framebuffer filter");
		return GL_NONE;
	}

	static GLint getWrap(FramebufferTextureWraping wrap){
		switch (wrap){
			case FramebufferTextureWraping::REPEATE: return GL_REPEAT;
			case FramebufferTextureWraping::MIROR_REPEATE: return GL_MIRRORED_REPEAT;
			case FramebufferTextureWraping::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
			case FramebufferTextureWraping::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		}
		ENGINE_CORE_ASSERT(false, "unknown framebuffer wrap mode");
		return GL_NONE;
	}

	static GLenum getTextureTarget(bool multisampled){
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static GLenum getAttachmentType(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::DEPTH24_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
			case FramebufferTextureFormat::DEPTH32F_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
		}
		return GL_DEPTH_ATTACHMENT;
	}
	
	static bool isFormatInt(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::R32I: return true;
			case FramebufferTextureFormat::RG32I: return true;
			case FramebufferTextureFormat::RGB32I: return true;
			case FramebufferTextureFormat::RGBA32I: return true;
			case FramebufferTextureFormat::R32F: return true;
			case FramebufferTextureFormat::RG32F: return true;
			case FramebufferTextureFormat::RGB32F: return true;
			case FramebufferTextureFormat::RGBA32F: return true;
			case FramebufferTextureFormat::R8UI: return true;
			case FramebufferTextureFormat::RG8UI: return true;
			case FramebufferTextureFormat::RGB8UI: return true;
			case FramebufferTextureFormat::RGBA8UI: return true;
			case FramebufferTextureFormat::R16UI: return true;
			case FramebufferTextureFormat::RG16UI: return true;
			case FramebufferTextureFormat::RGB16UI: return true;
			case FramebufferTextureFormat::RGBA16UI: return true;
			case FramebufferTextureFormat::R32UI: return true;
			case FramebufferTextureFormat::RG32UI: return true;
			case FramebufferTextureFormat::RGB32UI: return true;
			case FramebufferTextureFormat::RGBA32UI: return true;
			case FramebufferTextureFormat::R8I: return true;
			case FramebufferTextureFormat::RG8I: return true;
			case FramebufferTextureFormat::RGB8I: return true;
			case FramebufferTextureFormat::RGBA8I: return true;
		}
		return false;
	}

	static bool isFormatUnsignedInt(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::R32UI: return true;
			case FramebufferTextureFormat::RG32UI: return true;
			case FramebufferTextureFormat::RGB32UI: return true;
			case FramebufferTextureFormat::RGBA32UI: return true;
			case FramebufferTextureFormat::R8UI: return true;
			case FramebufferTextureFormat::RG8UI: return true;
			case FramebufferTextureFormat::RGB8UI: return true;
			case FramebufferTextureFormat::RGBA8UI: return true;
			case FramebufferTextureFormat::R16UI: return true;
			case FramebufferTextureFormat::RG16UI: return true;
			case FramebufferTextureFormat::RGB16UI: return true;
			case FramebufferTextureFormat::RGBA16UI: return true;
		}
		return false;
	}
	
	OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferLayout &layout){
		width = layout.width;
		height = layout.height;
		samples = layout.samples;

		ENGINE_CORE_ASSERT(width > 0, "cannot create a framebuffer with a nul width value");
		ENGINE_CORE_ASSERT(height > 0, "cannot create a framebuffer with a nul height value");
		ENGINE_CORE_ASSERT(width < GL_MAX_FRAMEBUFFER_WIDTH, "cannot create a framebuffer with a larger with than the maximal allowed (", width, " > ", GL_MAX_FRAMEBUFFER_WIDTH, ")");
		ENGINE_CORE_ASSERT(height < GL_MAX_FRAMEBUFFER_HEIGHT, "cannot create a framebuffer with a larger height than the maximal allowed (", height, " > ", GL_MAX_FRAMEBUFFER_HEIGHT, ")");
		ENGINE_CORE_ASSERT(samples < GL_MAX_FRAMEBUFFER_SAMPLES, "cannot create a framebuffer with more than ", GL_MAX_FRAMEBUFFER_SAMPLES, " samples (", samples, " > ", GL_MAX_FRAMEBUFFER_SAMPLES, ")");

		for (auto &attachment : layout.attachments){
			if (isFramebufferTextureFormatColor(attachment.format)){
				colorAttachmentsSpecs.push_back(attachment);
			} else {
				depthAttachmentSpecs = attachment;
			}
		}

		colorAttachments.resize(colorAttachmentsSpecs.size());

		ENGINE_PROFILE_FUNCTION();
		rebuild();
	}

	OpenGlFramebuffer::~OpenGlFramebuffer(){
		ENGINE_PROFILE_FUNCTION();
		destroyAttachments();
		glDeleteFramebuffers(1, &framebuffer);
	}

	void OpenGlFramebuffer::destroyAttachments(){
		ENGINE_PROFILE_FUNCTION();
		if (!colorAttachments.empty()) glDeleteTextures(colorAttachments.size(), colorAttachments.data());
		if (depthAttachment) glDeleteTextures(1, &depthAttachment);
	}

	void OpenGlFramebuffer::bind(){
		if (!isBind){
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glViewport(0, 0, width, height);
			isBind = true;
		}
	}

	void OpenGlFramebuffer::unBind(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		isBind = false;
	}

	void OpenGlFramebuffer::rebuild(){
		ENGINE_PROFILE_FUNCTION();

		if (framebuffer){
			destroyAttachments();
			glDeleteFramebuffers(1, &framebuffer);
			framebuffer = 0;

			memset(colorAttachments.data(), 0, colorAttachments.size());
			depthAttachment = 0;
		}

		glCreateFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		
		bool multisample = samples > 1;
		GLenum textureTarget = getTextureTarget(multisample);

		if (!colorAttachments.empty()){
			glCreateTextures(textureTarget, colorAttachments.size(), colorAttachments.data());

			for (int i=0; i<colorAttachmentsSpecs.size(); i++){
				auto& specs = colorAttachmentsSpecs[i];
				auto& attachment = colorAttachments[i];
				
				glBindTexture(textureTarget, attachment);

				if (multisample){
					glTexImage2DMultisample(textureTarget, samples, getInternalFormat(specs.format), width, height, GL_FALSE);
				} else {
					glTexImage2D(textureTarget, 0, getInternalFormat(specs.format), width, height, 0, getFormat(specs.format), GL_UNSIGNED_BYTE, nullptr);
				
					glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, getFiltering(specs.minificationFiltering));
					glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, getFiltering(specs.minificationFiltering));

					glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, getWrap(specs.wrap));
					glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, getWrap(specs.wrap));
					glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, getWrap(specs.wrap));
				}

				glFramebufferTexture2D(GL_FRAMEBUFFER, getGLColorAttachmentIndex(i), textureTarget, attachment, 0);
			}
		}

		if (depthAttachmentSpecs){
			glCreateTextures(textureTarget, 1, &depthAttachment);
			glBindTexture(textureTarget, depthAttachment);

			if (multisample){
				glTexImage2DMultisample(textureTarget, samples, getInternalFormat(depthAttachmentSpecs.format), width, height, GL_FALSE);
			} else {
				glTexStorage2D(textureTarget, 1, getInternalFormat(depthAttachmentSpecs.format), width, height);
				
				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, getFiltering(depthAttachmentSpecs.minificationFiltering));
				glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, getFiltering(depthAttachmentSpecs.minificationFiltering));
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, getWrap(depthAttachmentSpecs.wrap));
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, getWrap(depthAttachmentSpecs.wrap));
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, getWrap(depthAttachmentSpecs.wrap));
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, getAttachmentType(depthAttachmentSpecs.format), textureTarget, depthAttachment, 0);
		}

		if (colorAttachments.size() > 1){
			std::vector<GLenum> buffers(colorAttachments.size());

			for (size_t i=0; i<buffers.size(); i++)
				buffers[i] = GL_COLOR_ATTACHMENT0 + i;

			glDrawBuffers(buffers.size(), buffers.data());

		} else if (colorAttachments.empty()){
			// only depth
			glDrawBuffer(GL_NONE);
		}

		ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "framebuffer is incomplete : ", std::hex, glCheckFramebufferStatus(GL_FRAMEBUFFER));
		
		// avoid to setup the frame buffer as the swapchain's frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGlFramebuffer::resize(uint32_t width, uint32_t height){
		ENGINE_PROFILE_FUNCTION();
		this->width = width;
		this->height = height;
		rebuild();
	}

	uint32_t OpenGlFramebuffer::getColorAttachment(uint32_t slot) const{
		if (slot >= colorAttachments.size()) return 0;
		return colorAttachments[slot];
	}

	void OpenGlFramebuffer::getPixel(uint32_t attachment, uint32_t x, uint32_t y, void* pixel){
		ENGINE_CORE_ASSERT(attachment < colorAttachments.size(), "attchment index greater than the attachment count");
		// flip the y
		y = height - y;
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
		glReadPixels(x, y, 1, 1, getFormat(colorAttachmentsSpecs[attachment].format), getDataType(colorAttachmentsSpecs[attachment].format), pixel);
	}

	void OpenGlFramebuffer::clear(uint32_t attachment, void* clearData){
		ENGINE_CORE_ASSERT(attachment < colorAttachments.size(), "attchment index greater than the attachment count");
		glClearTexImage(colorAttachments[attachment], 0, getFormat(colorAttachmentsSpecs[attachment].format), getDataType(colorAttachmentsSpecs[attachment].format), clearData);
	}

}