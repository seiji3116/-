#pragma once

class Player;
class Enemy;

class MotherEnemy :public KdGameObject
{
public:
	MotherEnemy() { Init(); }
	~MotherEnemy() {}

	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void DrawBright()override;
	void Init()override;
	void Update()override;
	void PostUpdate()override;

	Math::Vector3 GetNodePos(std::string _nodeName ,int _num);

	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

	std::shared_ptr<Player> GetPlayer()
	{
		std::shared_ptr<Player> player = m_wpPlayer.lock();
		return player;
	}

	void PushBackLaserEnemyList(std::shared_ptr<Enemy> _enemy)
	{
		m_laserEnemyList.push_back(_enemy);
	}

	void Hit();
	bool GetGameClearFlg() { return m_gameClearFlg; }

private:
	std::shared_ptr<KdModelWork> m_model;

	std::weak_ptr<Player> m_wpPlayer;

	Math::Vector3 m_pos;
	Math::Vector3 m_moveDir;
	Math::Vector3 m_scale;
	Math::Matrix m_transMat;				// à⁄ìÆçsóÒ
	Math::Matrix m_rotateMat;				// âÒì]çsóÒ
	Math::Matrix m_scaleMat;				// ägèkçsóÒ

	std::list<std::shared_ptr<Enemy>> m_laserEnemyList;

	float m_missileCnt;
	float m_hitPoint;

	bool m_gameClearFlg;
};