#include "Enemy.h"
#include "../Player/Player.h"
#include "../../../Scene/SceneManager.h"

void Enemy::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	if (spPlayer)
	{
		m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();
		m_targetPos = spPlayer->GetPos();
		m_targetMat = spPlayer->GetMatrix();
		m_stateMan->SetPos(m_mWorld.Translation());
		m_stateMan->EnemyUpdate(*this);

		//if (m_stateMan->GetActionEndFlg())
		//{
		//	if (targetDir.Length() > 2)
		//	{
		//		m_stateMan->ChengeTracking();
		//		m_model = m_stateMan->GetModel();
		//	}
		//	else
		//	{
		//		m_stateMan->ChengeRush();
		//		m_model = m_stateMan->GetModel();
		//		m_stateMan->SetMoveDir(targetDir);
		//		m_stateMan->SetTargetDir(targetDir);
		//		m_stateMan->SetMatrix(m_mWorld);
		//	}
		//}
		//m_stateMan->SetTargetDir(targetDir);
	}

	//m_stateMan->SetMatrix(m_mWorld);
	//m_stateMan->Update();
	//m_mWorld = m_stateMan->GetMatrix();
}

void Enemy::PostUpdate()
{
}

bool Enemy::Hit()
{
	m_stateMan->Hit(*this);
	return false;

	////========================================================
	////球判定
	////========================================================
	//// 球判定用の変数
	//KdCollider::SphereInfo sphereInfo;

	//// 球の中心位置を設定
	//sphereInfo.m_sphere.Center = m_mWorld.Translation();

	//// 球の半径を設定
	//sphereInfo.m_sphere.Radius = 1.2f;

	//// 当たり判定をしたいタイプを設定
	//sphereInfo.m_type = KdCollider::TypeDamage;
	//// 弾の当たったオブジェクト情報を格納するリスト
	//std::list<KdCollider::CollisionResult> retSphereList;

	//// 球と当たり判定をする
	//// マップとの当たり判定
	//for (auto& obj : SceneManager::Instance().GetObjList())
	//{
	//	obj->Intersects
	//	(
	//		sphereInfo,
	//		&retSphereList
	//	);
	//}

	//// 急に当たったリスト情報から1番近いオブジェクトを検出
	//float maxOverLap = 0;
	//bool sphereHitFlg = false;
	//Math::Vector3 hitDir = Math::Vector3::Zero; // 当たった方向
	//for (auto& ret : retSphereList)
	//{
	//	// １番近くで当たったものを探す
	//	if (maxOverLap < ret.m_overlapDistance)
	//	{
	//		sphereHitFlg = true;
	//		maxOverLap = ret.m_overlapDistance;
	//		hitDir = ret.m_hitDir;
	//		hitDir.Normalize();
	//	}
	//}
	//if (sphereHitFlg)
	//{
	//	// 球とモデルが当たっている
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}

void Enemy::SetState(const std::string& _stateName)
{
	// ステートセット関数
	m_stateMan = std::make_unique<StateManager>();
	if (_stateName == "Normal")
	{
		m_stateMan->EnemyChengeNormal(*this);
		m_nowState = State::Normal;
	}
	else if(_stateName == "Laser")
	{
		m_stateMan->EnemyChengeLaser(*this);
		m_nowState = State::Laser;
	}
	else
	{
		return;
	}
}

void Enemy::SetDebugSphere(const Math::Vector3& _pos, const float _radius, const Math::Color& _color)
{
	m_pDebugWire->AddDebugSphere(_pos, _radius, _color);
}

void Enemy::SetDubugLine(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _length, const Math::Color& _color)
{
	m_pDebugWire->AddDebugLine(_pos, _dir, _length, _color);
}

const int Enemy::GetShieldNum() const
{
	return m_shieldNum;
}

void Enemy::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Enemy::DrawBright()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Enemy::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Enemy::DrawDebug()
{
	m_pDebugWire->Draw();
}

void Enemy::Init()
{
	m_moveDir = Math::Vector3(0, 0, 1);

	m_model = std::make_shared<KdModelWork>();
	m_pCollider = std::make_unique<KdCollider>();

	// デバッグワイヤーセット
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}