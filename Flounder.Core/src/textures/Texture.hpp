#pragma once

#include <string>
#include <vector>
#include "../platforms/Platform.hpp"
#include "../renderer/buffers/Buffer.hpp"
#include "../renderer/pipelines/Descriptor.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class Texture :
		public Buffer
	{
	private:
		std::string m_file;
		std::vector<std::string> m_cubemap;

		bool m_hasAlpha;
		bool m_clampEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;
		uint32_t m_numberOfRows;

		int32_t m_components;
		int32_t m_width, m_height, m_depth;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;
		VkImageType m_imageType;
	public:
		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		/// <param name="file"> The textures file. </param>
		Texture(const std::string &file, const bool &hasAlpha = false,
			const bool &clampEdges = false,
			const uint32_t &mipLevels = 1,
			const bool &anisotropic = true,
			const bool &nearest = false,
			const uint32_t &numberOfRows = 1);

		/// <summary>
		/// A new OpenGL cubemap texture object.
		/// </summary>
		/// <param name="cubemap"> The list of cubemap texture paths. </param>
		Texture(const std::vector<std::string> &cubemap);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) const;

		/// <summary>
		/// Gets if the texture has alpha.
		/// </summary>
		/// <returns> If the texture has alpha. </returns>
		bool HasAlpha() const { return m_hasAlpha; }

		/// <summary>
		/// Sets if the texture has alpha.
		/// </summary>
		/// <param name="hasAlpha"> If the texture has alpha. </param>
		void SetHasAlpha(const bool &hasAlpha) { m_hasAlpha = hasAlpha; }

		/// <summary>
		/// Gets the number of texture rows.
		/// </summary>
		/// <returns> The number of texture rows. </returns>
		int32_t GetNumberOfRows() const { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		void SetNumberOfRows(const int32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		/// <summary>
		/// The textures type.
		/// </summary>
		/// <returns> The textures type. </returns>
		VkImageType GetTextureType() const { return m_imageType; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static stbi_uc *LoadPixels(const std::string &filepath, int *width, int *height, int *components);
	private:
		/// <summary>
		/// Loads the texture object from a texture file.
		/// </summary>
		void CreateImage2D();

		/// <summary>
		/// Loads the texture object from a cubemap texture files.
		/// </summary>
		void CreateImage3D();

		void CreateImage(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory);

		void TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout);

		void CopyBufferToImage(const uint32_t &width, const uint32_t &height, const VkBuffer &buffer, const VkImage &image);

		static VkDeviceSize SomeShittyFunctionThatGetsATexturesSize(const std::string &file);
	};
}
