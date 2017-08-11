#include "filterdarken.h"

namespace flounder
{
	filterdarken::filterdarken(const float &factorValue) :
		ipostfilter("filterDarken", "res/shaders/filters/darken.frag.spv")
	{
		m_factorValue = factorValue;
	}

	filterdarken::filterdarken() :
		filterdarken(0.45f)
	{
	}

	filterdarken::~filterdarken()
	{
	}

	void filterdarken::storeValues()
	{
		m_shader->loadUniform1f("factor", m_factorValue);
	}
}
