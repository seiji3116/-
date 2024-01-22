#include "TrackingMissile.h"
#include "../../../Object/GameObject/Player/Player.h"
#include "../../../Scene/SceneManager.h"

void TrackingMissile::Init()
{
	// ���f���Ǎ�
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Missile/TrackingMissile.gltf"));

	// �ړ���
	m_moveSpd = 0.7f;
	m_targetDir = Math::Vector3::Zero;

	// �f�o�b�O���C���[������
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// �ǂ̃^�C�v�ɓ����邩�̐ݒ�
	m_collisionType = KdCollider::TypeDamage;

	//�����蔻�菉����
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("TrackingMissile", m_mWorld.Translation(), 5.f, KdCollider::TypeEnemyShot);
}

void TrackingMissile::Update()
{
	//m_pDebugWire->AddDebugSphere(m_mWorld.Translation(), 5.f);

	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (!spPlayer) { return; }

	if (spPlayer->GetGameClearFlg())
	{
		m_isExpired = true;

	}

	float playerLength = (spPlayer->GetPos() - m_mWorld.Translation()).Length();
	if (playerLength >= 200)
	{
		Hit();
	}

	// ���W�ړ�
	if (m_targetDir == Math::Vector3::Zero)
	{
		m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();
		// ���񂾂����ʂ��v���C���[�Ɋ��S�ɍ��킹��
		Rotation(m_targetDir, -1000.f, 1000.f);
	}
	m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();

	Math::Vector3 vMove = m_mWorld.Backward();
	vMove.Normalize();

	vMove *= m_moveSpd;
	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);
	m_mWorld *= trans;

	Rotation(m_targetDir, -0.025f, 0.025f);	// �^�[�Q�b�g�ւ̒ǔ�����
	Collision();							// �����蔻��

}

void TrackingMissile::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TrackingMissile::DrawDebug()
{
	m_pDebugWire->Draw();
}

void TrackingMissile::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TrackingMissile::SetTargetColliderType(KdCollider::Type type)
{
	m_collisionType = type;
}

void TrackingMissile::Hit()
{
	m_isExpired = true;
}

void TrackingMissile::Rotation(Math::Vector3& _targetDir, float _clampAngMin, float _clampAngMax)
{
	Math::Vector3 nowDir = m_mWorld.Backward();
	nowDir.Normalize();
	_targetDir.Normalize();

	float dot = nowDir.Dot(_targetDir);
	dot = std::clamp(dot, -1.f, 1.f);

	float betweeanAng = acos(dot);
	betweeanAng = DirectX::XMConvertToDegrees(betweeanAng);

	float rotateAng = std::clamp(betweeanAng, _clampAngMin, _clampAngMax);

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

void TrackingMissile::Collision()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (!spPlayer) { return; }
	if (spPlayer->GetGameClearFlg()) { return; }

	//========================================================
	//���C����
	//========================================================
	// ���C����p�ɕϐ����쐬
	KdCollider::RayInfo rayInfo;

	// ���C�̔��ˍ��W��ݒ�
	rayInfo.m_pos = m_mWorld.Translation();

	// ���C�̕�����ݒ�
	rayInfo.m_dir = m_mWorld.Backward();

	// ���C�̒�����ݒ�
	rayInfo.m_range = m_moveSpd;

	// �����蔻����������^�C�v��ݒ�
	rayInfo.m_type = m_collisionType;

	// ���C�ɓ��������I�u�W�F�N�g���
	std::list<KdCollider::CollisionResult> retRayList;

	// ���C�Ɠ����蔻�������
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			rayInfo,
			&retRayList
		);
	}

	// ���C�ɓ����������X�g����1�ԋ߂��I�u�W�F�N�g�����o
	float maxOverLap = 0;
	Math::Vector3 groundPos;
	bool hit = false;
	for (auto& ret : retRayList)
	{
		// ���C���Ւf���I�[�o�[��������
		// 1�Ԓ������̂�T��
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			groundPos = ret.m_hitPos;
			hit = true;
		}
	}

	// �������Ă�����
	if (hit)
	{
		Hit();
		spPlayer->Hit();
	}

	//========================================================
	//������
	//========================================================
	// ������p�̕ϐ�
	KdCollider::SphereInfo sphereInfo;

	// ���̒��S�ʒu��ݒ�
	sphereInfo.m_sphere.Center = m_mWorld.Translation();

	// ���̔��a��ݒ�
	sphereInfo.m_sphere.Radius = 1.5f;

	// �����蔻����������^�C�v��ݒ�
	sphereInfo.m_type = m_collisionType;

	// �f�o�b�O�p
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// ���������I�u�W�F�N�g�����i�[���郊�X�g
	std::list<KdCollider::CollisionResult> retSphereList;

	// ���Ɠ����蔻�������
	// �}�b�v�Ƃ̓����蔻��
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			sphereInfo,
			&retSphereList
		);
	}

	// �����������X�g��񂩂�1�ԋ߂��I�u�W�F�N�g�����o
	for (auto& ret : retSphereList)
	{
		hit = true;
	}
	if (hit)
	{
		// ���ƃ��f�����������Ă���
		Hit();
		spPlayer->Hit();
	}
}