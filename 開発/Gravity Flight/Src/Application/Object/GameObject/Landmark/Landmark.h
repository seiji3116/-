#pragma once

class Landmark :public KdGameObject
{
public:
	Landmark() { Init(); }
	~Landmark() {}

	void Init() override;
	void Update() override;
	void DrawLit() override;

	void PushBackPlanetMap(const std::string& _name, const Math::Vector3& _pos)
	{
		m_planetMap.find(_name)->second->m_mat.Translation(_pos);
	}
	
private:
	void StateSet(const std::string& _name, const std::string_view& _fileName);

	struct PlanetState
	{
		Math::Matrix m_mat;
		std::shared_ptr<KdModelWork> m_model;
		float m_angle;
		float m_angleSpd;
	};

	float m_scale;

	std::map<std::string, std::shared_ptr<PlanetState>> m_planetMap;
};