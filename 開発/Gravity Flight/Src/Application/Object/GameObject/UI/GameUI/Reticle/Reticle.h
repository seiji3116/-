#pragma once

class Player;

class Reticle: public KdGameObject
{
public:
	Reticle() { Init(); }
	~Reticle(){}

	void Init() override;
	void Update() override;
	void DrawSprite() override;

	void SetPos(const Math::Vector2& _pos)
	{
		m_pos = _pos;
	}

	void AliveOff() { m_isExpired = true; }

private:
	KdTexture m_tex;

	Math::Vector2 m_pos;
};