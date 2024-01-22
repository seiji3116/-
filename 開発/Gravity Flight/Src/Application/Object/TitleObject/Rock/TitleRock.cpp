#include "TitleRock.h"
#include "../Player/TitlePlayer.h"

void TitleRock::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Planet/Moon/Moon.gltf"));

	m_scale = Math::Vector3(50, 50, 50);
	m_angle = 0;

	m_handle = KdEffekseerManager::GetInstance().Play("Gravity/GravityObj.efk", m_mWorld.Translation());
}

void TitleRock::Update()
{
	Math::Matrix playerTrans = Math::Matrix::Identity;
	const std::shared_ptr<TitlePlayer> spPlayer = m_player.lock();

	if (spPlayer)
	{
		playerTrans = Math::Matrix::CreateTranslation(spPlayer->GetPos());
	}

	//自転
	m_rotateMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(2.0f));

	//公転(移動してから回転)
	m_axis -= Math::Vector3(0, 0, 0.001f);
	m_axis.Normalize();
	m_transMat *= Math::Matrix::CreateFromAxisAngle(m_axis ,DirectX::XMConvertToRadians(0.5f));

	//行列の合成(最後に親の行列を合成する！)=行列の親子関係
	m_mWorld = m_rotateMat * m_transMat * playerTrans;

	KdEffekseerManager::GetInstance().SetWorldMatrix(m_handle, m_mWorld);
	KdEffekseerManager::GetInstance().SetScale(m_handle, 0.3f);
}

void TitleRock::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TitleRock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}
