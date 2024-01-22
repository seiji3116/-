#include "TitlePlayer.h"

void TitlePlayer::Init()
{
	//// �����ݒ�
	//m_mWorld.Translation(Math::Vector3(0, 0, 0));

	// ���f���̓Ǎ�
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player3.gltf"));
}

void TitlePlayer::Update()
{
	float y;
	SinCurve(&y, 2);

	m_mWorld.Translation(Math::Vector3(0, y, 0));
}

void TitlePlayer::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}
