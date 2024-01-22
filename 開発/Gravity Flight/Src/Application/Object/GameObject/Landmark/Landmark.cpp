#include "Landmark.h"

void Landmark::Init()
{
	StateSet("Landmark1", "Asset/Models/Planet/Earth/Earth.gltf");
	StateSet("Landmark2", "Asset/Models/Planet/Mars/Mars.gltf");
	StateSet("Landmark3", "Asset/Models/Planet/Neptune/Neptune.gltf");
	StateSet("Landmark4", "Asset/Models/Planet/Uranus/Uranus.gltf");
	StateSet("Landmark5", "Asset/Models/Planet/Venus/Venus.gltf");

	m_scale = 45;
}

void Landmark::Update()
{
	for (auto& map : m_planetMap)
	{
		map.second->m_angle += map.second->m_angleSpd;
		if (map.second->m_angle >= 360)
		{
			map.second->m_angle -= 360;
		}
	}
}

void Landmark::DrawLit()
{
	Math::Matrix mat;
	for (auto& map : m_planetMap)
	{
		map.second->m_mat = Math::Matrix::CreateScale(m_scale) 
							* Math::Matrix::CreateRotationY(map.second->m_angle)
							* Math::Matrix::CreateTranslation(map.second->m_mat.Translation());
		KdShaderManager::Instance().m_HD2DShader.DrawModel(*map.second->m_model, map.second->m_mat);
	}
}

void Landmark::StateSet(const std::string& _name, const std::string_view& _fileName)
{
	std::shared_ptr<PlanetState> state = std::make_shared<PlanetState>();
	state->m_model = std::make_shared<KdModelWork>();
	state->m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData(_fileName));
	state->m_angleSpd = (float)Random(5, 10) / 1000;
	m_planetMap[_name] = state;
}
