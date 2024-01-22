#include "EnemyTracking.h"
#include "../../../Object/GameObject/Enemy/Enemy.h"

void EnemyTracking::EnemyInit(Enemy& _owner)
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/NormalEnemy.gltf"));

	_owner.SetModel(m_model);

	m_moveSpd = 0.5f;
	m_actionEndFlg = true;
}

void EnemyTracking::EnemyUpdate(Enemy& _owner)
{
	Math::Vector3 vMove = m_mWorld.Backward();
	vMove.Normalize();

	vMove *= m_moveSpd;
	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);
	m_mWorld *= trans;

	Rotate(_owner.GetTargetDir());

	_owner.SetMatrix(m_mWorld);
}

void EnemyTracking::EnemyShot(Enemy& _owner)
{
	return;
}

void EnemyTracking::EnemyAction(Enemy& _owner)
{
	return;
}

void EnemyTracking::Hit(Enemy& _owner)
{
}

void EnemyTracking::Rotate(Math::Vector3 _targetDir)
{
	Math::Vector3 nowDir = m_mWorld.Backward();
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

	Math::Vector3 pos = m_mWorld.Translation();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}
