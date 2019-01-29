#pragma once

#include "Helpers/String.hpp"
#include "Models/Model.hpp"

namespace acid
{
	class ACID_EXPORT ModelRectangle :
		public Model
	{
	public:
		static std::shared_ptr<ModelRectangle> Create(const float &min, const float &max);

		static std::shared_ptr<ModelRectangle> Create(const std::string &data);

		ModelRectangle(const float &width, const float &height);

		void Encode(Metadata &metadata) const override;
	private:
		static std::string ToName(const float &min, const float &max);

		float m_min;
		float m_max;
	};
}
