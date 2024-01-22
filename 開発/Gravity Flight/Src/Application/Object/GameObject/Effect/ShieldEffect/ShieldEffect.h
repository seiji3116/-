#pragma once

class ShieldEffect :public KdGameObject
{
public:
	ShieldEffect() { Init(); }
	~ShieldEffect() {}

	void Init() override;
	void Update() override;

	void SetEffectName(const std::string _name)
	{
		m_efkName = _name;
	}
	void StopEffect();

private:
	//void Collision();
	std::shared_ptr<KdGameObject> m_owner;
	bool m_firstPlay;
	std::string m_efkName = "";

	Effekseer::Handle m_handle = 100000;
};