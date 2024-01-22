#include "Bullet.h"

void Bullet::Update()
{
	m_moveSpd += 0.01f;
	if (m_moveSpd > 1)
	{
		m_moveSpd = 1;
	}

	m_moveDir.Normalize();
	m_pos += m_moveDir * m_moveSpd;
}

void Bullet::PostUpdate()
{
	UpdateRotate();

	m_transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_rotateMat * m_transMat;
}

void Bullet::UpdateRotate()
{
	// 角度制御
	Math::Vector3 vFront = Math::Vector3::Zero;
	Math::Vector3 vHeadUp = Math::Vector3::Zero;
	Math::Vector3 vRight = Math::Vector3::Zero;

	// 移動したい方向を前方向として保存
	vFront = m_moveDir;

	// 前方向から90度Z回転したベクトルを頭上方向として保存
	vHeadUp = vFront;
	Math::Matrix mRot90 = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90.0f));
	vHeadUp = vHeadUp.TransformNormal(vHeadUp, mRot90);

	// 前方向と頭上方向の外積を右方向ベクトルとして保存
	vRight = vHeadUp.Cross(vFront);

	// それぞれのベクトルの正規化
	vFront.Normalize();
	vRight.Normalize();
	vHeadUp.Normalize();

	// Rotateの各方向に対応するベクトルを入れる
	m_rotateMat.Backward(vFront);
	m_rotateMat.Up(vHeadUp);
	m_rotateMat.Right(vRight);
}

void Bullet::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Bullet::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Bullet::DrawBright()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Bullet::Init()
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Missile/Missile.gltf"));

	// 座標移動系変数の初期化
	m_pos = Math::Vector3::Zero;
	m_moveDir = Math::Vector3::Zero;
	m_moveSpd = 0;

	// マトリックスの初期化
	m_rotateMat = Math::Matrix::Identity;
	m_transMat = Math::Matrix::Identity;

	//当たり判定初期化
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageModel", m_model, KdCollider::TypeDamage);
}