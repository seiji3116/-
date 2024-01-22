#include "EnemyNormal.h"
#include "../../../Object/GameObject/Bullet/Bullet.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Object/GameObject/Enemy/Enemy.h"

void EnemyNormal::EnemyInit(Enemy& _owner)
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/NormalEnemy.gltf"));
	_owner.SetModel(m_model);

	m_moveSpd = 0.5f;
	m_shotCnt = 100.0f;
	m_targetDistance = 10.0f;	// ターゲットからどれだけ離れて弾を撃つか

	//m_addAng.x = Random(1, 3);
	m_addAng.x = 1;

	m_localTransMat = Math::Matrix::CreateTranslation(0, 0, -20.0f);
}

void EnemyNormal::EnemyUpdate(Enemy& _owner)
{
	Rotate();
	Math::Matrix targetMat = Math::Matrix::CreateTranslation(_owner.GetTargetPos());

	m_rotateMat = GetRotationMatrix();

	m_mWorld = m_localTransMat * m_rotateMat * targetMat;

	//UpdatePos(_owner.GetTargetPos());
	_owner.SetMatrix(m_mWorld);
	EnemyShot(_owner);
}

void EnemyNormal::EnemyShot(Enemy& _owner)
{
	m_shotCnt--;
	if (m_shotCnt <= 0)
	{
		m_shotCnt = 100.0f;
		std::shared_ptr<Bullet> bullet;
		bullet = std::make_shared<Bullet>();
		bullet->SetPos(m_mWorld.Translation());
		bullet->SetMoveDir(_owner.GetTargetDir());
		SceneManager::Instance().AddObject(bullet);
	}
}

void EnemyNormal::EnemyAction(Enemy& _owner)
{
	EnemyShot(_owner);
	UpdatePos(_owner.GetTargetPos());
}

void EnemyNormal::Hit(Enemy& _owner)
{
}

void EnemyNormal::Rotate()
{
	//m_DegAng += m_addAng;
	//if (m_DegAng.x >= 360)
	//{
	//	m_DegAng.x -= 360;
	//}
}

void EnemyNormal::UpdatePos(const Math::Vector3& _targetPos)
{
	//// カメラの最新データ
	//m_ghostTargetPos[0] = _targetPos;

	//// 最後から1つ前のデータをコピー
	//for (int i = GHOST_MAX - 1; i > 0; i--)
	//{
	//	m_ghostTargetPos[i] = m_ghostTargetPos[i - 1];
	//}

	//Math::Matrix transMat = Math::Matrix::CreateTranslation(m_ghostTargetPos[30]);
	//m_mWorld = transMat;
}

int EnemyNormal::Random(int _min, int _max)
{
	std::mt19937 engine{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(_min, _max);
	auto n = dist(engine);
	return n;
}
