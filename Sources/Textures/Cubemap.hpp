﻿#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/Descriptor.hpp"
#include "Resources/Resource.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded cubemap texture.
	/// </summary>
	class ACID_EXPORT Cubemap :
		public Descriptor,
		public Resource
	{
	public:
		static std::shared_ptr<Cubemap> Create(const Metadata &metadata);

		/// <summary>
		/// Will find an existing cubemap with the same filename, or create a new cubemap.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileSuffix"> The files suffix type (ex .png). </param>
		/// <param name="filter"> The type of filtering will be use on the texture. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		static std::shared_ptr<Cubemap> Create(const std::string &filename, const std::string &fileSuffix, const VkFilter &filter = VK_FILTER_LINEAR,
			const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, const bool &anisotropic = true, const bool &mipmap = true);

		/// <summary>
		/// A new cubemap object.
		/// </summary>
		/// <param name="filename"> The file base name (path without extension or face name). </param>
		/// <param name="fileSuffix"> The files extension type (ex .png). </param>
		/// <param name="filter"> The type of filtering will be use on the texture. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		explicit Cubemap(const std::string &filename, const std::string &fileSuffix = ".png", const VkFilter &filter = VK_FILTER_LINEAR,
			const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT, const bool &anisotropic = true, const bool &mipmap = true);

		/// <summary>
		/// A new cubemap object from a array of pixels.
		/// </summary>
		/// <param name="width"> The cubemaps width. </param>
		/// <param name="height"> The cubemaps height. </param>
		/// <param name="pixels"> The initial pixels to use in the texture. <seealso cref="#GetPixels()"/> to get a copy of the pixels, and <seealso cref="#SetPixels()"/> to set the pixels. </param>
		/// <param name="format"> The cubemaps format. </param>
		/// <param name="imageLayout"> The cubemaps image layout </param>
		/// <param name="usage"> The cubemaps image usage </param>
		/// <param name="filter"> The type of filtering will be use on the cubemap. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		Cubemap(const uint32_t &width, const uint32_t &height, void *pixels = nullptr, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, const VkFilter &filter = VK_FILTER_LINEAR, const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT, const bool &anisotropic = false, const bool &mipmap = false);

		~Cubemap();

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count);

		WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType,
			const VkDescriptorSet &descriptorSet, const std::optional<OffsetSize> &offsetSize) const override;

		void Encode(Metadata &metadata) const override;

		/// <summary>
		/// Gets a copy of the face of a cubemaps pixels from memory, after usage is finished remember to delete the result.
		/// </summary>
		/// <param name="arrayLayer"> The layer to copy from. </param>
		/// <returns> A copy of the cubemaps pixels. </returns>
		uint8_t *GetPixels(const uint32_t &arrayLayer) const;

		/// <summary>
		/// Gets a copy of the cubemaps pixels from memory, after usage is finished remember to delete the result.
		/// </summary>
		/// <returns> A copy of the cubemaps. </returns>
		uint8_t *GetPixels() const;

		/// <summary>
		/// Copies the pixels into this cubemaps memory.
		/// </summary>
		/// <param name="pixels"> The pixels to copy to the image. </param>
		void SetPixels(const uint8_t *pixels);

		const std::string &GetFilename() const { return m_filename; };

		const std::string &GetFileSuffix() { return m_fileSuffix; };

		const VkFilter &GetFilter() const { return m_filter; }

		const VkSamplerAddressMode &GetAddressMode() const { return m_addressMode; }

		const bool &IsAnisotropic() const { return m_anisotropic; }

		const uint32_t &GetMipLevels() const { return m_mipLevels; }

		const VkSampleCountFlagBits &GetSamples() const { return m_samples; }

		const VkImageLayout &GetImageLayout() const { return m_imageLayout; }

		const uint32_t &GetComponents() const { return m_components; }

		const uint32_t &GetWidth() const { return m_width; }

		const uint32_t &GetHeight() const { return m_height; }

		const VkImage &GetImage() const { return m_image; }

		const VkImageView &GetImageView() const { return m_imageView; }

		const VkSampler &GetSampler() const { return m_sampler; }
	private:
		std::string m_filename;
		std::string m_fileSuffix;

		VkFilter m_filter;
		VkSamplerAddressMode m_addressMode;
		bool m_anisotropic;
		uint32_t m_mipLevels;
		VkSampleCountFlagBits m_samples;
		VkImageLayout m_imageLayout;

		uint32_t m_components;
		uint32_t m_width, m_height;

		VkImage m_image;
		VkDeviceMemory m_deviceMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;
	};
}
