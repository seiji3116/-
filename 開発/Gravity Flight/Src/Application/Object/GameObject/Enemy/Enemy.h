#pragma once
#include "../../../StatePattern/StateManager.h"

class Player;

class Enemy : public KdGameObject
{
public:

	Enemy() { Init(); };
	~Enemy() {};

	void GenerateDepthMapFromLight()override;
	void DrawBright()				override;
	void DrawLit()					override;
	void DrawDebug()				override;
	void Init()						override;
	void Update()					override;
	void PostUpdate()				override;

	bool Hit();

	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

	void SetState(const std::string& _stateName);

	void SetDebugSphere(const Math::Vector3& _pos, const float _radius, const Math::Color& _color);
	void SetDubugLine(const Math::Vector3& _pos, const Math::Vector3& _dir, const float _length, const Math::Color& _color);
	void SetShieldNum(const int _num)
	{
		m_shieldNum = _num;
	}
	void SetShieldFlg(const bool _flg)
	{
		m_shieldFlg = _flg;
	}

	// add: ターゲット関係
	virtual Math::Vector3 GetTargetDir() const { return m_targetDir; }
	virtual Math::Vector3 GetTargetPos() const { return m_targetPos; }
	const bool GetShieldFlg() const { return m_shieldFlg; }
	const Math::Matrix GetTargetMat() const { return m_targetMat; }

	const int GetShieldNum() const;

private:
	enum State
	{
		Tracking,
		Normal,
		Laser
	};

private:
	std::unique_ptr<StateManager> m_stateMan;

	std::weak_ptr<Player> m_wpPlayer;

	// add: ターゲット関係
	Math::Matrix	m_targetMat;
	Math::Vector3	m_targetDir;
	Math::Vector3	m_targetPos;

	Math::Vector3	m_moveDir;
	Math::Vector3	m_scale;
	Math::Matrix	m_transMat;				// 移動行列
	Math::Matrix	m_rotateMat;			// 回転行列
	Math::Matrix	m_scaleMat;				// 拡縮行列

	float m_roll = 0;						// z軸用回転量
	float m_pitch = 0;						// x軸用回転量

	// シールド関係
	int m_shieldNum = 0;
	bool m_shieldFlg = false;

	State m_nowState;						// 現在のステート
};