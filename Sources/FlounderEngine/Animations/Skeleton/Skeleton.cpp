#include "Skeleton.hpp"

#include "../MeshAnimated.hpp"

namespace Flounder
{

	Skeleton::Skeleton(LoadedValue *parent, std::vector<std::string> boneOrder) :
		m_armatureData(nullptr),
		m_boneOrder(new std::vector<std::string>(boneOrder)),
		m_jointCount(0),
		m_skeletonData(nullptr)
	{
		for (auto child : parent->GetChild("visual_scene")->GetChild("node")->m_children)
		{
			if (child->GetChild("-id")->m_value == "Armature")
			{
				m_armatureData = child;
				break;
			}
		}


		auto headNode = m_armatureData->GetChild("node");
		JointData *headJoint = LoadJointData(headNode, true);
		m_skeletonData = new SkeletonData(m_jointCount, headJoint);
	}

	Skeleton::~Skeleton()
	{
		delete m_boneOrder;

		delete m_skeletonData;
	}

	JointData *Skeleton::LoadJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		JointData *joint = ExtractMainJointData(jointNode, isRoot);

		for (auto childNode : jointNode->GetChild("node")->m_children)
		{
			joint->AddChild(LoadJointData(childNode, false));
		}

		return joint;
	}

	JointData *Skeleton::ExtractMainJointData(LoadedValue *jointNode, const bool &isRoot)
	{
		std::string nameId = jointNode->GetChild("-id")->m_value;
		ptrdiff_t index = std::find(m_boneOrder->begin(), m_boneOrder->end(), nameId) - m_boneOrder->begin();
		std::vector<std::string> matrixData = FormatString::Split(jointNode->GetChild("matrix")->GetChild("#text")->m_value, " ");
		Matrix4 matrix = ConvertData(matrixData);
		matrix.Transpose();

		if (isRoot)
		{
			// Because in Blender z is up, but in our game y is up.
			Matrix4::Multiply(*MeshAnimated::S_CORRECTION, matrix, &matrix);
		}

		m_jointCount++;
		return new JointData(static_cast<int>(index), nameId, matrix);
	}

	Matrix4 Skeleton::ConvertData(const std::vector<std::string> &rawData)
	{
		float *data = new float[16];

		for (unsigned int i = 0; i < rawData.size(); i++)
		{
			data[i] = FormatString::ConvertTo<float>(rawData.at(i));
		}

		Matrix4 result = Matrix4(data);
		delete[] data;
		return result;
	}
}
