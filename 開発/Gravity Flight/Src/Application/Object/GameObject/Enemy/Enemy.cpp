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
	////������
	////========================================================
	//// ������p�̕ϐ�
	//KdCollider::SphereInfo sphereInfo;

	//// ���̒��S�ʒu��ݒ�
	//sphereInfo.m_sphere.Center = m_mWorld.Translation();

	//// ���̔��a��ݒ�
	//sphereInfo.m_sphere.Radius = 1.2f;

	//// �����蔻����������^�C�v��ݒ�
	//sphereInfo.m_type = KdCollider::TypeDamage;
	//// �e�̓��������I�u�W�F�N�g�����i�[���郊�X�g
	//std::list<KdCollider::CollisionResult> retSphereList;

	//// ���Ɠ����蔻�������
	//// �}�b�v�Ƃ̓����蔻��
	//for (auto& obj : SceneManager::Instance().GetObjList())
	//{
	//	obj->Intersects
	//	(
	//		sphereInfo,
	//		&retSphereList
	//	);
	//}

	//// �}�ɓ����������X�g��񂩂�1�ԋ߂��I�u�W�F�N�g�����o
	//float maxOverLap = 0;
	//bool sphereHitFlg = false;
	//Math::Vector3 hitDir = Math::Vector3::Zero; // ������������
	//for (auto& ret : retSphereList)
	//{
	//	// �P�ԋ߂��œ����������̂�T��
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
	//	// ���ƃ��f�����������Ă���
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}
}

void Enemy::SetState(const std::string& _stateName)
{
	// �X�e�[�g�Z�b�g�֐�
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

	// �f�o�b�O���C���[�Z�b�g
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}