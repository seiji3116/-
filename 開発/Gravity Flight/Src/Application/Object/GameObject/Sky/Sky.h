#pragma once

class Player;

class Sky :public KdGameObject
{
public:
	Sky() { Init(); }
	~Sky() {}

	void Update()override;
	void DrawUnLit()override;
	void GenerateDepthMapFromLight()override;
	void Init()override;

	Math::Vector3 GetNodePos(std::string _nodeName);

	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

private:
	std::shared_ptr<KdModelWork> m_model;

	std::weak_ptr<Player> m_wpPlayer;

	Math::Vector3 m_pos;
	Math::Vector3 m_scale;
	Math::Matrix m_transMat;
	Math::Matrix m_rotateMat;
	Math::Matrix m_scaleMat;
};