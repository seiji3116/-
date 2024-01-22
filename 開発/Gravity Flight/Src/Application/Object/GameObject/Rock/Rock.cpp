#include "Rock.h"
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../../../Scene/SceneManager.h"

void Rock::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (!spPlayer) { return; }
	m_playerVec = spPlayer->GetPos() - m_mWorld.Translation();

	if (spPlayer->GetGravityFlg())
	{
		if (m_playerVec.Length() < 5 && !m_gravityFlg)
		{
			if (spPlayer->PushBackGravityList(this))
			{
				m_gravityFlg = true;
				m_handle = KdEffekseerManager::GetInstance().Play("Gravity/GravityObj.efk", m_mWorld.Translation());
			}
		}
	}

	Gravity();
	Throw();
	Collishon();
}

void Rock::Throw()
{
	if (!m_throwFlg) { return; }

	if (KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_handle, m_mWorld);
		KdEffekseerManager::GetInstance().SetScale(m_handle, 0.3f);
	}

	static Math::Vector3 debugPos;
	if (!m_debugFlg)
	{
		m_debugFlg = true;
		debugPos = m_mWorld.Translation();
	}

	m_targetDir.Normalize();

	m_pDebugWire->AddDebugLine(debugPos, m_targetDir, 100);

	Math::Vector3 pos = GetPos();

	pos += m_targetDir * m_gravityPow;

	m_mWorld.Translation(pos);
}

void Rock::Gravity()
{
	if (!m_gravityFlg) { return; }

	if (KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		KdEffekseerManager::GetInstance().SetWorldMatrix(m_handle, m_mWorld);
		KdEffekseerManager::GetInstance().SetScale(m_handle, 0.3f);
	}

	Math::Vector3 vMove = m_mWorld.Forward();
	vMove.Normalize();

	vMove *= 0.2;
	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);
	m_mWorld *= trans;

	Rotate(m_playerVec);
}

void Rock::Collishon()
{
	// ìäÇ∞ÇÁÇÍÇƒÇ¢Ç»Ç©Ç¡ÇΩÇÁì¸ÇÁÇ»Ç¢
	if (!m_throwFlg) { return; }

	for (auto& laserEnemy : m_laserEnemyList)
	{
		Math::Vector3 vec = laserEnemy->GetPos() - m_mWorld.Translation();
		if (!laserEnemy->IsExpired())
		{
			if (!laserEnemy->GetShieldFlg())
			{
				if (vec.Length() <= 6.5f)
				{
					laserEnemy->Hit();
					Math::Vector3 dir = Math::Vector3(Random(0, 1), Random(0, 1), Random(0, 1));
					dir.Normalize();
					vec.Normalize();
					m_targetDir = -vec + dir;
					m_targetDir.Normalize();
					KdEffekseerManager::GetInstance().StopEffect(m_handle);
					return;
				}
			}
			else
			{
				if (vec.Length() <= 4.f && !laserEnemy->GetInvincibleFlg())
				{
					laserEnemy->Hit();
					m_isExpired = true;
					KdEffekseerManager::GetInstance().StopEffect(m_handle);
				}
			}
		}
	}
}

void Rock::Rotate(Math::Vector3 _targetDir)
{
	Math::Vector3 nowDir = m_mWorld.Forward();
	nowDir.Normalize();
	_targetDir.Normalize();

	float dot = nowDir.Dot(_targetDir);
	dot = std::clamp(dot, -1.f, 1.f);

	float betweeanAng = acos(dot);
	betweeanAng = DirectX::XMConvertToDegrees(betweeanAng);

	float rotateAng = std::clamp(betweeanAng, -4.f, 4.f);

	Math::Vector3 rotAxis;
	nowDir.Cross(_targetDir, rotAxis);
	if (rotAxis.LengthSquared() == 0)return;

	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}

void Rock::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Rock::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Rock::DrawBright()
{
}

void Rock::DrawDebug()
{
	m_pDebugWire->Draw();
}

void Rock::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Planet/Moon/Moon.gltf"));

	m_scale = Math::Vector3(50, 50, 50);
	m_angle = 0;

	m_gravityPow = 0.5f;
	m_gravityFlg = false;

	m_throwFlg = false;

	m_localPos = Math::Vector3(2, 0, 0);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	//ìñÇΩÇËîªíËèâä˙âª
	m_pCollider = std::make_unique<KdCollider>();
	//m_pCollider->RegisterCollisionShape("RockModel", m_mWorld.Translation(), 2, KdCollider::TypeEvent);
}
