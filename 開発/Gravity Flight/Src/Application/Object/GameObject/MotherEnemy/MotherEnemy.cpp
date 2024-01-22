#include "MotherEnemy.h"
#include "../../../Object/GameObject/TrackingMissile/TrackingMissile.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../../../Scene/SceneManager.h"

void MotherEnemy::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void MotherEnemy::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void MotherEnemy::DrawBright()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void MotherEnemy::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	if (spPlayer)
	{
		m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();
		m_targetPos = spPlayer->GetPos();
		m_missileCnt--;
		if (!m_gameClearFlg)
		{
			if (m_missileCnt <= 0)
			{
				m_missileCnt = 300.0f;
				std::shared_ptr trackingMissile = std::make_shared<TrackingMissile>();
				trackingMissile->SetPos(GetNodePos("MissilePos", 0));
				trackingMissile->SetPlayer(spPlayer);
				trackingMissile->SetMotherEnemyPos(m_mWorld.Translation());
				SceneManager::Instance().AddObject(trackingMissile);
			}
		}
	}

	// すべてのレーザーエネミーが消えていたら
	float cnt = 0;
	for (auto& enemy : m_laserEnemyList)
	{
		if (enemy->IsExpired())
		{
			cnt++;
		}
	}
	if (cnt >= 3)
	{
		//Hit();
		m_isExpired = false;
		m_gameClearFlg = true;
	}
}

void MotherEnemy::PostUpdate()
{
}

Math::Vector3 MotherEnemy::GetNodePos(std::string _nodeName, int _num)
{
	std::string nodeNum;
	const KdModelWork::Node* pNode;

	if (_nodeName == "LaserPos")
	{
		nodeNum = std::to_string(_num);
		pNode = m_model->FindNode(_nodeName + nodeNum);
	}
	else
	{
		nodeNum = std::to_string(_num);
		pNode = m_model->FindNode(_nodeName);
	}


	if (pNode)
	{
		return pNode->m_worldTransform.Translation() + m_mWorld.Translation();
	}
	return Math::Vector3::Zero;
}

void MotherEnemy::Hit()
{
	m_hitPoint--;
	if (m_hitPoint <= 0)
	{
		m_isExpired = false;
		m_gameClearFlg = true;
	}
}

void MotherEnemy::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/MotherEnemy.gltf"));

	m_missileCnt = 300.0f;
	m_gameClearFlg = false;

	m_pos = Math::Vector3::Zero;
	m_transMat = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, 150));
	m_rotateMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	m_scaleMat = Math::Matrix::Identity;

	m_mWorld = m_scaleMat * m_rotateMat * m_transMat;
}