#pragma once

class ResultUI :public KdGameObject
{
public:
	ResultUI() { Init(); }
	~ResultUI() {}

	void Init() override;
	void Update() override;
	void DrawSprite() override;

	// ResultópÉtÉâÉOêÿÇËë÷Ç¶
	void GameClearFlgOn()	
	{ 
		if (!m_gameClearFlg)
		{
			m_gameClearFlg = true;
		}
	}
	void GameOverFlgOn()	
	{ 
		if (!m_gameOverFlg)
		{
			m_gameOverFlg = true;
		}
	}

private:
	KdTexture m_gameClearTex;
	KdTexture m_gameOverTex;
	KdTexture m_clickTex;

	bool m_gameClearFlg = false;
	bool m_gameOverFlg = false;

	float m_clickAlpha;
};