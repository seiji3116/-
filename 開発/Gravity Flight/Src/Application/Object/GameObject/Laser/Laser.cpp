#include "Laser.h"
#include "../Camera/TPSCamera/TPSCamera.h"

void Laser::DrawBright()
{
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_poly, m_mWorld);
}

void Laser::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_poly, m_mWorld);
}

void Laser::Init()
{
	m_poly = std::make_shared<KdSquarePolygon>();
	m_poly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/GameUI/Laser/Laser.png"));

	m_mWorld = Math::Matrix::Identity;
	m_centerPos = Math::Vector3::Zero;
	m_startPos = Math::Vector3::Zero;
	m_dir = Math::Vector3(1, 1, 0);
	m_range = Math::Vector2(1, 1);
}

void Laser::Update()
{
	UpdateCenterPos();
	Rotate(m_dir);
	m_poly->SetScale(m_range);
}

void Laser::PostUpdate()
{
}

void Laser::Rotate(Math::Vector3 _dir)
{
	Math::Vector3 nowDir = m_mWorld.Up();
	nowDir.Normalize();
	_dir.Normalize();

	float dot = nowDir.Dot(_dir);
	dot = std::clamp(dot, -1.f, 1.f);

	float rotateAng = DirectX::XMConvertToDegrees(acos(dot));

	Math::Vector3 rotAxis;
	nowDir.Cross(_dir, rotAxis);
	if (rotAxis.LengthSquared() == 0)return;

	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	m_mWorld.Translation(Math::Vector3(0, 0, 0));
	
	//Billboard(rotation);

	m_mWorld *= rotation;
	m_mWorld.Translation(m_centerPos);
}

void Laser::UpdateCenterPos()
{
	float xHalfRange = m_range.y / 2;
	m_dir.Normalize();
	m_centerPos = (m_dir * xHalfRange) + m_startPos;
}
