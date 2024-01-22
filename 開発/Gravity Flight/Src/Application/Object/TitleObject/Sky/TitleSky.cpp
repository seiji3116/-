#include "TitleSky.h"

void TitleSky::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/sky.gltf"));
}

void TitleSky::Update()
{
}

void TitleSky::DrawUnLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}
