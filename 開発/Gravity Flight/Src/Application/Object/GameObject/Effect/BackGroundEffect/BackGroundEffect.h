#pragma once

class BackGroundEffect :public KdGameObject
{
public:
	BackGroundEffect() { Init(); }
	~BackGroundEffect(){}

	void Init() override;
	void Update() override;

	void SetEffectName(const std::string _name)
	{
		m_efkName = _name;
	}

private:

	int m_rePlayCnt;
	bool m_firstPlayFlg;
	std::string m_efkName = "";

	Effekseer::Handle m_handle = 100000;
};