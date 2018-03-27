#include "FilterSepia.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			Texture::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterSepia::FilterSepia(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Sepia.frag", graphicsStage, DESCRIPTORS)
	{
	}

	FilterSepia::~FilterSepia()
	{
	}

	void FilterSepia::Render(const VkCommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
		}

		m_descriptorSet->Update({
			m_pipeline->GetTexture(2),
			m_pipeline->GetTexture(2)
		});

		// Draws the object.
		m_pipeline->BindPipeline(commandBuffer);

		m_descriptorSet->BindDescriptor(commandBuffer);
		m_model->CmdRender(commandBuffer);
	}
}
