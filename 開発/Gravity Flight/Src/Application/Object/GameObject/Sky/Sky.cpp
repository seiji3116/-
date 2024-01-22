#include "Sky.h"
#include "../Player/Player.h"

void Sky::Update()
{

}

Math::Vector3 Sky::GetNodePos(std::string _nodeName)
{
	const KdModelWork::Node* pNode = m_model->FindNode(_nodeName);

	if (pNode)
	{
		return pNode->m_worldTransform.Translation() + m_mWorld.Translation();
	}
	return Math::Vector3::Zero;
}

void Sky::DrawUnLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Sky::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Sky::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/sky.gltf"));

	/*m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Sky", m_model, KdCollider::TypeEvent);*/
}