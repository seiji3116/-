#pragma once

class Player;

class ConcentratioLine : public KdGameObject
{
public:
	ConcentratioLine() { Init(); }
	~ConcentratioLine() {}

	void Init() override;
	void Update() override;
	void DrawSprite() override;

	void SetAccelFlg(bool _flg) { m_accelFlg = _flg; }
	void AliveOff() { m_isExpired = true; }

private:
	KdTexture m_tex;
	std::weak_ptr<Player> m_wpPlayer;

	Math::Vector2 m_pos;
	float m_alpha;
	bool m_accelFlg;
};