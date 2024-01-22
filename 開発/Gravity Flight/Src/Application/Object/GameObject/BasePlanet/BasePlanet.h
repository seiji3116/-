#pragma once

class Base :public KdGameObject
{
public:
	Base() { Init(); }
	~Base(){}

	void Init() override;
	void DrawLit() override;
	void Update() override;

private:
};