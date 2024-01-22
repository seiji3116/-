#pragma once

class TitlePlayer;

class TitleRock :public KdGameObject
{
public:
	TitleRock() { Init(); }
	~TitleRock(){}

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void GenerateDepthMapFromLight() override;

	void SetPlayer(std::shared_ptr<TitlePlayer> _player) { m_player = _player; }
	void SetLocalTrans(const Math::Vector3 _pos) { m_transMat = Math::Matrix::CreateTranslation(_pos); }
	void SetAxis(const Math::Vector3 _axis) { m_axis = _axis; }

private:
	// エフェクト
	Effekseer::Handle m_handle = 100000;

	std::weak_ptr<TitlePlayer> m_player;

	Math::Vector3 m_axis = Math::Vector3::Zero;

	Math::Vector3 m_scale;
	Math::Matrix m_transMat = Math::Matrix::Identity;
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	Math::Matrix m_revolutionMat = Math::Matrix::Identity;
	float m_angle = 0;
	Math::Vector3 m_ownerPos = Math::Vector3::Zero;
	Math::Vector3 m_revolutionPos = Math::Vector3::Zero;
};