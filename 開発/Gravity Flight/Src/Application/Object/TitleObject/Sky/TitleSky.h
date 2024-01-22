#pragma once

class TitleSky :public KdGameObject
{
public:
	TitleSky() { Init(); }
	~TitleSky(){}

	void Init() override;
	void Update() override;
	void DrawUnLit() override;

private:
};