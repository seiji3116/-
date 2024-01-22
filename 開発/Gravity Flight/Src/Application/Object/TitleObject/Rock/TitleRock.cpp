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

	//���]
	m_rotateMat *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(2.0f));

	//���](�ړ����Ă����])
	m_axis -= Math::Vector3(0, 0, 0.001f);
	m_axis.Normalize();
	m_transMat *= Math::Matrix::CreateFromAxisAngle(m_axis ,DirectX::XMConvertToRadians(0.5f));

	//�s��̍���(�Ō�ɐe�̍s�����������I)=�s��̐e�q�֌W
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
