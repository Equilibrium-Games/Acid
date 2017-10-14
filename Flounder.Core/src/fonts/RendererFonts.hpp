#pragma once

#include "../renderer/IRenderer.hpp"
#include "Text.hpp"

namespace Flounder
{
	class RendererFonts :
		public IRenderer
	{
	private:
	public:
		RendererFonts();

		~RendererFonts();

		void Render(const VkCommandBuffer *commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
