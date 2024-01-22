#include "TitleUI.h"

void TitleUI::Init()
{
	m_alpha = 1;
	m_nameTex.Load("Asset/Textures/UI/TitleUI/GravityFlight.png");
	m_clickTex.Load("Asset/Textures/UI/Utility/Click.png");
}

void TitleUI::Update()
{
	SinCurveAlpha(&m_alpha, 2);
}

void TitleUI::DrawSprite()
{
	Math::Rectangle rc;
	Math::Color color;
	Math::Matrix mat;

	// タイトル
	color = Math::Color(1, 1, 1, 1);
	rc = Math::Rectangle(0, 0, 898, 95);
	mat = Math::Matrix::CreateTranslation(Math::Vector3(0, 250, 0));
	KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_nameTex, 0, 0, 898, 95, &rc, &color);

	// クリック
	color = Math::Color(1, 1, 1, m_alpha);
	rc = Math::Rectangle(0, 0, 327, 95);
	mat = Math::Matrix::CreateTranslation(Math::Vector3(0, -250, 0));
	KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_clickTex, 0, 0, 327, 95, &rc, &color);

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}

void TitleUI::Release()
{
	m_nameTex.Release();
	m_clickTex.Release();
}
