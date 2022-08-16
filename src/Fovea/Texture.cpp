#include "Fovea/Texture.hpp"
#include "Fovea/core.hpp"
#include "Fovea/SingleTimeCommand.hpp"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Fovea{
	Texture::Texture(Framebuffer &framebuffer, uint32_t attachment, TextureBuilder& builder){
		if (attachment == DEPTH_ATTACHMENT){
			image = framebuffer.depthImage;
			image.custom = true;

			imageView = framebuffer.depthImageView;
			imageView.custom = true;

			imageMemory = framebuffer.depthImageMemory;
			imageMemory.custom = true;

			format = framebuffer.depthFormat;
		} else {
			auto &a = framebuffer.colorAttachments[attachment];
			image = a.image;
			imageView = a.imageView;
			imageMemory = a.imageMemory;
			format = a.format;
		}

		extent = framebuffer.extent;
		createImageSampler(builder);
	}

	static inline VkFormat channelCountToVkFormat(int channel){
		switch (channel){
			case 1: return VK_FORMAT_R8_UINT;
			case 2: return VK_FORMAT_R8G8_UINT;
			case 3: return VK_FORMAT_R8G8B8_UINT;
			case 4: return VK_FORMAT_R8G8B8A8_UINT;
		}
		return VK_FORMAT_R8_UINT;
	}

	Texture::Texture(const std::filesystem::path &path, TextureBuilder& builder){
		int w, h, channel;
		void* pixels = stbi_load(path.string().c_str(), &w, &h, &channel, STBI_default);

		extent.width = static_cast<uint32_t>(w);
		extent.height = static_cast<uint32_t>(h);
		format = channelCountToVkFormat(channel);

		createFromData(pixels, channel, builder);

		stbi_image_free(pixels);
	}

	Texture::Texture(VkFormat format, VkExtent2D extent, void* data, uint32_t pixelSize, TextureBuilder& builder){
		this->extent = extent;
		this->format = format;

		createFromData(data, pixelSize, builder);
	}

	Texture::~Texture(){
		VkDevice device = getInstance().logicalDevice.getDevice();
		if (!image.custom) vkDestroyImage(device, image, nullptr);
		if (!imageView.custom) vkDestroyImageView(device, imageView, nullptr);
		if (!imageMemory.custom) vkFreeMemory(device, imageMemory, nullptr);
		vkDestroySampler(device, imageSampler, nullptr);
	}

	void Texture::createImageSampler(TextureBuilder& builder){
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = builder.magFilter;
		samplerInfo.minFilter = builder.minFilter;

		samplerInfo.addressModeU = builder.addressModeU;
		samplerInfo.addressModeV = builder.addressModeV;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		
		samplerInfo.anisotropyEnable = builder.anisotropy;
		samplerInfo.maxAnisotropy = getInstance().physicalDevice.getProperties().limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = !builder.normalizeCoords;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.f;
		samplerInfo.minLod = 0.f;
		samplerInfo.maxLod = 0.f;

		if (vkCreateSampler(getInstance().logicalDevice.getDevice(), &samplerInfo, nullptr, &imageSampler) != VK_SUCCESS)
			throw std::runtime_error("failed to create sampler");
	}

	void Texture::createImage(TextureBuilder& builder){
		this->extent = extent;

		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.extent.width = extent.width;
		createInfo.extent.height = extent.height;
		createInfo.extent.depth = 1;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.format = format;
		createInfo.tiling = builder.tiling;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.usage =  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.samples = builder.samples;
		createInfo.flags = 0;

		getInstance().logicalDevice.createImageWithInfo(createInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, imageMemory);
	}

	void Texture::createImageView(TextureBuilder& builder){
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.format = format;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &imageView) != VK_SUCCESS){
			throw std::runtime_error("failed to create image view");
		}
	}

	void Texture::createFromData(void* data, uint32_t pixelSize, TextureBuilder& builder){
		createImage(builder);
		createImageView(builder);

		Buffer staginBuffer;
		VkDeviceSize imageSize = extent.width * extent.height * pixelSize;
		staginBuffer.alloc(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		staginBuffer.map();
		staginBuffer.write(data);
		staginBuffer.flush();
		staginBuffer.unmap();

		SingleTimeCommand::transitionImageLayout(image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		SingleTimeCommand::copyBufferToImage(staginBuffer.getBuffer(), image, extent.width, extent.height);
		SingleTimeCommand::transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		createImageSampler(builder);
	}

}