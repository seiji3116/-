#include "ConcentratioLine.h"

void ConcentratioLine::Init()
{
	m_pos = Math::Vector2::Zero;
	m_alpha = 0;
	m_accelFlg = false;
	m_tex.Load("Asset/Textures/UI/GameUI/ConcentrationLine.png");
}

void ConcentratioLine::Update()
{
	if (m_accelFlg)
	{
		m_alpha += 0.05f;
		if (m_alpha >= 1)
		{
			m_alpha = 1;
		}
	}
	if (!m_accelFlg)
	{
		m_alpha -= 0.05f;
		{
			if (m_alpha <= 0)
			{
				m_alpha = 0;
			}
		}
	}
}

void ConcentratioLine::DrawSprite()
{
	m_pos.x = Random(-5, 5);
	m_pos.y = Random(-5, 5);

	Math::Rectangle rc;
	Math::Color color;
	Math::Matrix mat;

	color = Math::Color(1, 1, 1, m_alpha);
	rc = Math::Rectangle(0, 0, 1280, 720);
	mat = Math::Matrix::CreateTranslation(Math::Vector3(m_pos.x, m_pos.y, 0));
	KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, 0, 0, 1280, 720, &rc, &color);
}
