#include "ResultUI.h"

void ResultUI::Init()
{
	m_clickAlpha = 1;
	m_clickTex.Load("Asset/Textures/UI/Utility/Click.png");

	m_gameClearTex.Load("Asset/Textures/UI/ResultUI/GameClear.png");
	m_gameOverTex.Load("Asset/Textures/UI/ResultUI/GameOver.png");
}

void ResultUI::Update()
{
	// アルファ値をサインカーブで変える
	SinCurveAlpha(&m_clickAlpha, 2);
}

void ResultUI::DrawSprite()
{
	Math::Rectangle rc;
	Math::Color color;
	Math::Matrix mat;

	// クリック
	if (m_gameClearFlg || m_gameOverFlg)
	{
		color = Math::Color(1, 1, 1, m_clickAlpha);
		rc = Math::Rectangle(0, 0, 327, 95);
		mat = Math::Matrix::CreateTranslation(Math::Vector3(0, -250, 0));
		KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_clickTex, 0, 0, 327, 95, &rc, &color);
	}

	// クリアフラグが付いたらclearの画面を映す
	if (m_gameClearFlg)
	{
		color = Math::Color(1, 1, 1, 1);
		rc = Math::Rectangle(0, 0, 690, 95);
		mat = Math::Matrix::CreateTranslation(Math::Vector3(0, 150, 0));
		KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_gameClearTex, 0, 0, 690, 95, &rc, &color);
	}

	// ゲームオーバーフラグが付いたらgameoverの画面を映す
	if (m_gameOverFlg)
	{
		color = Math::Color(1, 1, 1, 1);
		rc = Math::Rectangle(0, 0, 628, 95);
		mat = Math::Matrix::CreateTranslation(Math::Vector3(0, 150, 0));
		KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
		KdShaderManager::Instance().m_spriteShader.DrawTex(&m_gameOverTex, 0, 0, 628, 95, &rc, &color);
	}

	KdShaderManager::Instance().m_spriteShader.SetMatrix(Math::Matrix::Identity);
}
