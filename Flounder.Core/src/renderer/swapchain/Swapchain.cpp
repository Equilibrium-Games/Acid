﻿#include "Swapchain.hpp"

#include "../../devices/Display.hpp"
#include "../../shadows/Shadows.hpp"

namespace Flounder
{
	Swapchain::Swapchain(const VkExtent2D &extent) :
		m_presentMode(VK_PRESENT_MODE_FIFO_KHR),
		m_swapchain(VK_NULL_HANDLE),
		m_swapchainImageCount(0),
		m_swapchinImages(std::vector<VkImage>()),
		m_swapchinImageViews(std::vector<VkImageView>()),
		m_colourImage(nullptr),
		m_normalImage(nullptr),
		m_shadowImage(nullptr),
		m_extent({})
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto physicalDevice = Display::Get()->GetPhysicalDevice();
		const auto surface = Display::Get()->GetSurface();
		const auto surfaceFormat = Display::Get()->GetSurfaceFormat();
		const auto surfaceCapabilities = Display::Get()->GetSurfaceCapabilities();

		m_extent = extent;

		uint32_t physicalPresentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount, nullptr);
		std::vector<VkPresentModeKHR> physicalPresentModes(physicalPresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &physicalPresentModeCount,
			physicalPresentModes.data());

		for (auto presentMode : physicalPresentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_presentMode = presentMode;
			}
		}

		if (m_swapchainImageCount < surfaceCapabilities.minImageCount + 1)
		{
			m_swapchainImageCount = surfaceCapabilities.minImageCount + 1;
		}

		if (surfaceCapabilities.maxImageCount > 0)
		{
			if (m_swapchainImageCount > surfaceCapabilities.maxImageCount)
			{
				m_swapchainImageCount = surfaceCapabilities.maxImageCount;
			}
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = m_swapchainImageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = m_presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		Platform::ErrorVk(vkCreateSwapchainKHR(logicalDevice, &swapchainCreateInfo, nullptr, &m_swapchain));

		Platform::ErrorVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, nullptr));

		m_swapchinImages.resize(m_swapchainImageCount);
		m_swapchinImageViews.resize(m_swapchainImageCount);

		Platform::ErrorVk(vkGetSwapchainImagesKHR(logicalDevice, m_swapchain, &m_swapchainImageCount, m_swapchinImages.data()));

		for (uint32_t i = 0; i < m_swapchainImageCount; i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = m_swapchinImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = surfaceFormat.format;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange = {};
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			Platform::ErrorVk(vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &m_swapchinImageViews[i]));
		}

		m_colourImage = new Texture(Display::Get()->GetWidth(), Display::Get()->GetHeight(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		m_normalImage = new Texture(Display::Get()->GetWidth(), Display::Get()->GetHeight(), VK_FORMAT_R16G16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		m_shadowImage = new Texture(8192, 8192, VK_FORMAT_R16_SFLOAT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT); // TODO: Shadows::Get()->GetShadowSize(), Shadows::Get()->GetShadowSize() // 8192, 8192
	}

	Swapchain::~Swapchain()
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();

		delete m_shadowImage;
		delete m_normalImage;
		delete m_colourImage;

		for (auto imageView : m_swapchinImageViews)
		{
			vkDestroyImageView(logicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(logicalDevice, m_swapchain, nullptr);
	}
}
