#pragma once

#include "Framebuffer.hpp"
#include "../vulkan/vulkan.h"

#include <filesystem>

namespace Fovea{
	struct TextureBuilder{
		VkFilter magFilter = VK_FILTER_LINEAR;
		VkFilter minFilter = VK_FILTER_LINEAR;
		VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		bool anisotropy = false;
		bool normalizeCoords = false;

		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
	};

	class Texture{
		public:
			static constexpr uint32_t DEPTH_ATTACHMENT = -1;

			Texture(Framebuffer &framebuffer, uint32_t attachment, TextureBuilder& builder);
			Texture(const std::filesystem::path &path, TextureBuilder& builder);
			Texture(VkFormat format, VkExtent2D extent, void* data, uint32_t pixelSize, TextureBuilder& builder);

			~Texture();

		private:
			void createFromData(void* data, uint32_t pixelSize, TextureBuilder& builedr);

			void createImageSampler(TextureBuilder& builder);
			void createImage(TextureBuilder& builder);
			void createImageView(TextureBuilder& builder);

			template<typename T>
			struct Member{
				Member<T>(const T &other) {member = other;}
				Member<T>& operator=(const T &other) {member = other; return *this;}
				operator T&() {return member;}
				T& operator*() {return member;}
				T* operator->() {return &member;}
				T* operator&() {return &member;}

				T member;
				bool custom = false;
			};

			VkExtent2D extent;
			VkFormat format;

			Member<VkImage> image = VK_NULL_HANDLE;
			Member<VkDeviceMemory> imageMemory = VK_NULL_HANDLE;
			Member<VkImageView> imageView = VK_NULL_HANDLE;
			VkSampler imageSampler = VK_NULL_HANDLE;
	};
}