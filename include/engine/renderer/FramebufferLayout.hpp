#pragma once

#include "../core.hpp"
#include "../pch.hpp"

namespace engine{

	enum class FramebufferTextureFormat{
		NONE = 0,

		R8,
		RG8,
		RGB8,
		RGBA8,

		R8I,
		RG8I,
		RGB8I,
		RGBA8I,

		R8UI,
		RG8UI,
		RGB8UI,
		RGBA8UI,
		
		R16UI,
		RG16UI,
		RGB16UI,
		RGBA16UI,
		
		R32UI,
		RG32UI,
		RGB32UI,
		RGBA32UI,

		R16,
		RG16,
		RGB16,
		RGBA16,

		R16F,
		RG16F,
		RGB16F,
		RGBA16F,

		R32F,
		RG32F,
		RGB32F,
		RGBA32F,

		R32I,
		RG32I,
		RGB32I,
		RGBA32I,

		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8,

		DEPTH16,
		DEPTH24,
		DEPTH32,
		DEPTH32F,

		DEPTH = DEPTH24_STENCIL8
	};


	static bool isFramebufferTextureFormatDepth(FramebufferTextureFormat format){
		switch (format){
			case FramebufferTextureFormat::DEPTH: return true;
		}
		return false;
	}

	static bool isFramebufferTextureFormatColor(FramebufferTextureFormat format){
		if (format == FramebufferTextureFormat::NONE) return false;
		return !isFramebufferTextureFormatDepth(format);
	}

	enum class FramebufferTextureFiltering{
		NONE = 0,
		NEAREST = 1,
		LINEAR = 2
	};

	enum class FramebufferTextureWraping{
		NONE = 0,
		REPEATE = 1,
		MIROR_REPEATE = 2,
		CLAMP_TO_EDGE = 3,
		CLAMP_TO_BORDER = 4
	};

	struct ENGINE_API FramebufferTextureSpecification{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : format{format} {}
		FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferTextureFiltering mag) : format{format}, magnificationFiltering{mag} {}
		FramebufferTextureSpecification(FramebufferTextureFormat format, FramebufferTextureFiltering mag, FramebufferTextureFiltering min) : format{format}, magnificationFiltering{mag}, minificationFiltering{min} {}

		FramebufferTextureFormat format = FramebufferTextureFormat::NONE;
		FramebufferTextureFiltering magnificationFiltering = FramebufferTextureFiltering::NEAREST;
		FramebufferTextureFiltering minificationFiltering = FramebufferTextureFiltering::LINEAR;
		FramebufferTextureWraping wrap = FramebufferTextureWraping::REPEATE;

		operator bool() const {return format != FramebufferTextureFormat::NONE;}
	};

	struct ENGINE_API FramebufferAttachmentSpecification{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> list) : attachments{list}{}
		std::vector<FramebufferTextureSpecification> attachments;

		

		inline std::vector<FramebufferTextureSpecification>::iterator begin() {return attachments.begin();}
		inline std::vector<FramebufferTextureSpecification>::iterator end() {return attachments.end();}
		inline std::vector<FramebufferTextureSpecification>::const_iterator begin() const {return attachments.begin();}
		inline std::vector<FramebufferTextureSpecification>::const_iterator end() const {return attachments.end();}
	};

	struct ENGINE_API FramebufferLayout{
		uint32_t width, height;
		FramebufferAttachmentSpecification attachments;
		uint32_t samples = 1;
	};
}