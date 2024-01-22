#pragma once

class TitleUI :public KdGameObject
{
public:
	TitleUI() { Init(); }
	~TitleUI() { Release(); }

	void Init() override;
	void Update() override;
	void DrawSprite() override;
	void Release();

private:
	KdTexture m_nameTex;
	KdTexture m_clickTex;

	float m_alpha;
};