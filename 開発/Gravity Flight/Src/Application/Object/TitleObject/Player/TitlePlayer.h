#pragma once

class TitlePlayer :public KdGameObject
{
public:
	TitlePlayer() { Init(); }
	~TitlePlayer(){}

	void Init() override;
	void Update() override;
	void DrawLit() override;

private:
};