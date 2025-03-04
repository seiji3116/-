#include "BasePlanet.h"

void Base::Init()
{
	Math::Matrix transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, -150));
	Math::Matrix scaleMat = Math::Matrix::CreateScale(50);
	m_mWorld = scaleMat * transMat;

	// モデルの読込
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Planet/Jupiter/Jupiter.gltf"));

	//当たり判定初期化
	//m_pCollider = std::make_unique<KdCollider>();
	//m_pCollider->RegisterCollisionShape("BaseModel", m_model, KdCollider::TypeEvent);
}

void Base::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Base::Update()
{
}
