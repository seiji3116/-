#include "Player.h"
#include "../Camera/TPSCamera/TPSCamera.h"
#include "../Rock/Rock.h"
#include "../UI/GameUI/Reticle/Reticle.h"
#include "../UI/GameUI/ConcentratioLine/ConcentratioLine.h"

#include "../../../Scene/SceneManager.h"

void Player::Update()
{
	SetTarget();
	if (m_actionFlg)
	{
		Action();
	}
	GravityThrow();

	// �Q�[���I�[�o�[�ɂȂ�����A�N�V�������Ƃ߂�
	if (m_gameOverFlg)
	{
		m_actionFlg = false;
		m_moveSpd -= 0.1f;
		if (m_moveSpd <= 0)
		{
			m_moveSpd = 0;
		}
	}
	//RayCollision(m_mWorld.Translation(), m_targetDir, 5, KdCollider::TypeEvent);
	//SphereCollision(m_mWorld.Translation(), 5, KdCollider::TypeEvent);
}

void Player::PostUpdate()
{
	Math::Matrix rotMatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_roll));
	Math::Matrix rotMatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_pitch));

	Math::Matrix rotMat = rotMatX * rotMatZ;

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_moveVec * m_moveSpd);

	m_mWorld = rotMat * transMat * m_mWorld;
	m_moveDir = m_mWorld.Backward();
}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawBright()
{
	//KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawDebug()
{
	m_pDebugWire->Draw();
}

void Player::Init()
{
	// ���W�ړ��n������
	m_moveVec = Math::Vector3(0, 0, 0.1f);		// �ړ���
	m_moveDir = Math::Vector3::Zero;
	m_moveSpd = 1.5f;

	// �}�g���b�N�X�̏�����
	Math::Matrix transMat = Math::Matrix::CreateTranslation({ 0,5,0 });
	m_mWorld = transMat;

	m_gravityCnt = 30.f;
	m_hitPoint = 5;

	// �t���O�֌W������
	m_keyFlg = false;
	m_gravityKeyFlg = false;
	m_gravityFlg = false;
	m_throwKeyFlg = false;
	m_accelFlg = false;
	m_actionFlg = true;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// ���f���̓Ǎ�
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player3.gltf"));

	//�����蔻�菉����
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerModel", m_model, KdCollider::TypeDamage);
}

void Player::Hit()
{
	if (m_hitPoint >= 0)
	{
		m_hitPoint--;
		KdAudioManager::Instance().Play("Asset/Sounds/Game/PlayerDamage.wav", false, 0.3f);
	}
	if(m_hitPoint <= 0)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/Game/Bom.wav", false, 0.5f);
		KdEffekseerManager::GetInstance().Play("ToonHit/ToonHit.efk", m_mWorld.Translation(), false);
		m_isExpired = true;
		m_gameOverFlg = true;
		std::shared_ptr<Reticle> spReticle = m_reticle.lock();
		spReticle->AliveOff();
	}
}

void Player::Action()
{
	// ���[�J�����Ǘ�
	Math::Vector2 inputMove = KdInputManager::Instance().GetAxisState("move");
	m_roll -= inputMove.x * 0.1f;
	m_pitch -= inputMove.y * 0.1f;

	if (m_roll > 2)
	{
		m_roll = 2;
	}
	if (m_roll > 0)
	{
		m_roll -= 0.05f;
		if (m_roll < 0)
		{
			m_roll = 0;
		}
	}

	if (m_roll < -2)
	{
		m_roll = -2;
	}
	if (m_roll < 0)
	{
		m_roll += 0.05f;
		if (m_roll > 0)
		{
			m_roll = 0;
		}
	}

	if (m_pitch < -2)
	{
		m_pitch = -2;
	}
	if (m_pitch < 0)
	{
		m_pitch += 0.05f;
		if (m_pitch > 0)
		{
			m_pitch = 0;
		}
	}

	if (m_pitch > 2)
	{
		m_pitch = 2;
	}
	if (m_pitch > 0)
	{
		m_pitch -= 0.05f;
		if (m_pitch < 0)
		{
			m_pitch = 0;
		}
	}

	// ����
	std::shared_ptr<ConcentratioLine> conLine = m_concentratioLine.lock();
	if (KdInputManager::Instance().IsPress("accel"))
	{
		if (!m_keyFlg)
		{
			m_keyFlg = true;
			if (m_accelFlg)
			{
				m_accelFlg = false;
			}
			else
			{
				m_accelFlg = true;
			}
			conLine->SetAccelFlg(m_accelFlg);
		}
	}
	else
	{
		m_keyFlg = false;
	}

	if (m_accelFlg)
	{
		m_moveSpd += 0.1f;
		if (m_moveSpd >= 4)
		{
			m_moveSpd = 4;
		}
	}
	else
	{
		m_moveSpd -= 0.1f;
		if (m_moveSpd <= 1.5f)
		{
			m_moveSpd = 1.5f;
		}
	}
}

bool Player::RayCollision(const Math::Vector3& _centerPos, const Math::Vector3& _dir, float _range, KdCollider::Type _type)
{
	//========================================================
	//���C����
	//========================================================
	// ���C����p�ɕϐ����쐬
	KdCollider::RayInfo rayInfo;

	// ���C�̔��ˍ��W��ݒ�
	rayInfo.m_pos = _centerPos;

	// ���C�̕�����ݒ�
	Math::Vector3 dir = _dir;
	dir.Normalize();
	rayInfo.m_dir = dir;

	// ���C�̒�����ݒ�
	rayInfo.m_range = _range;

	// �����蔻����������^�C�v��ݒ�
	rayInfo.m_type = _type;

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
	bool hit = false;
	m_overlapDistance = 0.0f;
	for (auto& ret : retRayList)
	{
		// ���C���Ւf���I�[�o�[��������
		// 1�Ԓ������̂�T��
		if (m_overlapDistance < ret.m_overlapDistance)
		{
			m_overlapDistance = ret.m_overlapDistance;
			m_hitPos = ret.m_hitPos;
			m_hitDir = ret.m_hitDir;
			hit = true;
		}
	}

	// �������Ă�����
	if (hit)
	{
		return true;
	}
	return false;
}

bool Player::SphereCollision(const Math::Vector3& _centerPos, const float _radius, KdCollider::Type _type)
{
	//========================================================
	//������
	//========================================================
	// ������p�̕ϐ�
	KdCollider::SphereInfo sphereInfo;

	// ���̒��S�ʒu��ݒ�
	sphereInfo.m_sphere.Center = _centerPos;

	// ���̔��a��ݒ�
	sphereInfo.m_sphere.Radius = _radius;

	// �����蔻����������^�C�v��ݒ�
	sphereInfo.m_type = _type;

	// �f�o�b�O�p
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

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
	bool hit = false;
	m_overlapDistance = 0.0f;
	for (auto& ret : retSphereList)
	{
		// �P�ԋ߂��œ����������̂�T��
		if (m_overlapDistance < ret.m_overlapDistance)
		{
			m_overlapDistance = ret.m_overlapDistance;
			m_hitPos = ret.m_hitPos;
			m_hitDir = ret.m_hitDir;
			hit = true;
		}
	}
	if (hit)
	{
		// ���ƃ��f�����������Ă���
		return true;
	}
	// �e�Ɠ������Ă��Ȃ�������false�ŋA��
	return false;
}

void Player::GravityThrow()
{
	if (m_handle != 100000)
	{
		KdEffekseerManager::GetInstance().SetPos(m_handle, m_mWorld.Translation());
	}

	// ��Ȃǂ̈�����
	if (KdInputManager::Instance().IsPress("gravity"))
	{
		if (!m_gravityKeyFlg)
		{
			m_gravityKeyFlg = true;
			// ���X�g�ɃI�u�W�F�N�g��1���Ȃ��A�d�͑���\��Ԃ�������
			if (!m_gravityFlg && m_gravityList.size() <= 0)
			{
				KdAudioManager::Instance().Play("Asset/Sounds/Game/Gravity.wav", false, 0.3f);
				m_handle = KdEffekseerManager::GetInstance().Play("Gravity/Gravity.efk", m_mWorld.Translation(), false);
				m_gravityFlg = true;
			}
			else
			{
				if (m_gravityList.size() == 0)
				{
					m_gravityList.clear();
					m_gravityFlg = false;
					return;
				}
				// ���X�g�̍ŏ��̃I�u�W�F�N�g�𓊂���
				auto& it = m_gravityList.begin();
				KdAudioManager::Instance().Play("Asset/Sounds/Game/GravityThrow.wav", false, 0.5f);
				(*m_gravityList.begin())->ChengeThrow(m_targetDir);
				m_gravityList.erase(it);

			}
		}
	}
	else
	{
		m_gravityKeyFlg = false;
	}

	// ��莞�Ԍo�߂���Əd�͑���\��Ԃֈڍs
	m_gravityCnt--;
	if (m_gravityCnt <= 0)
	{
		m_gravityFlg = false;
		m_gravityCnt = 30.f;
	}
}

void Player::SetTarget()
{
	// �^�[�Q�b�g�ɂł���I�u�W�F�N�g�̒�����^�[�Q�b�g����I�u�W�F�N�g���w��
	const std::shared_ptr<CameraBase> spCamera = m_camera.lock();

	if (spCamera)
	{
		Math::Vector3 rayDir = Math::Vector3::Zero;
		float range = 100;

		// ���e�B�N����2D���W��3D���W�ɕϊ�
		std::shared_ptr<Reticle> spReticle = m_reticle.lock();
		POINT reticlePos = { spReticle->GetPos().x + 640, spReticle->GetPos().y + 360 };
		spCamera->WorkCamera()->GenerateRayInfoFromClientPos(reticlePos, spCamera->GetPos(), rayDir, range);

		m_pDebugWire->AddDebugLine(m_mWorld.Translation(), rayDir, range);

		// ���C�ɓ��������炠�����Ƃ�����^�[�Q�b�g�Ƃ��Ďw��
		if (RayCollision(m_mWorld.Translation(), rayDir, range, KdCollider::TypeTarget))
		{
			m_targetDir = m_hitDir;
			m_targetPos = m_hitPos;
		}
		// �������ĂȂ�������3D�ɕϊ������x�N�g�����w��
		else
		{
			m_targetDir = rayDir;
			m_targetPos = m_targetDir * range;
		}
	}
}
