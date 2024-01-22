#pragma once

class Player;
class Enemy;

class Rock : public KdGameObject
{
public:

	Rock() { Init(); };
	~Rock() {};

	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void DrawBright()override;
	void DrawDebug() override;
	void Init()override;
	void Update()override;

	void SetOwnerPos(const Math::Vector3& _pos) { m_ownerPos = _pos; }
	void SetRevolutionPos(const Math::Vector3& _pos) { m_revolutionPos = _pos; }
	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

	void PushBuckLaserEnemyList(std::shared_ptr<Enemy> _enemy)
	{
		m_laserEnemyList.push_back(_enemy);
	}

private:
	void Throw();
	void Gravity();
	void Collishon();
	void Rotate(Math::Vector3 _targetDir);

private:

	std::shared_ptr<KdModelWork> m_model;

	std::weak_ptr<Player> m_wpPlayer;

	Math::Vector3 m_scale;
	Math::Matrix m_transMat = Math::Matrix::Identity;
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	Math::Matrix m_revolutionMat = Math::Matrix::Identity;
	float m_angle;
	Math::Vector3 m_ownerPos;
	Math::Vector3 m_revolutionPos;

	// エフェクト
	Effekseer::Handle m_handle = 100000;

	// プレイヤーに引き寄せられるとき
	Math::Vector3 m_playerVec;		// プレイヤーへのベクトル
	Math::Vector3 m_targetVec;		// ターゲットの方向
	Math::Vector3 m_targetPos;		// ターゲットの座標
	Math::Vector3 m_localPos;		// ターゲットからどのくらい離れているか

	// レーザーの敵情報
	std::list<std::shared_ptr<Enemy>> m_laserEnemyList;

	bool m_debugFlg = false;
};