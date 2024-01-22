#include "Reticle.h"
#include "../../../../../Object/GameObject/Player/Player.h"
#include "../../../../../Object//GameObject/Camera/CameraBase.h"

void Reticle::Init()
{
	m_tex.Load("Asset/Textures/UI/GameUI/Reticle/Reticle.png");
	m_pos = Math::Vector2::Zero;
	m_mWorld = Math::Matrix::CreateTranslation(Math::Vector3(m_pos.x, m_pos.y, 0));
}

void Reticle::Update()
{

}

void Reticle::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0);
}
