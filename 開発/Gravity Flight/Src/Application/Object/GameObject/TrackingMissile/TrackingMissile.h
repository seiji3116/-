#pragma once

class Player;
class Enemy;

class TrackingMissile :public KdGameObject
{
public:
	TrackingMissile() { Init(); }
	~TrackingMissile(){}

	void Init() override;
	void Update() override;
	void DrawLit() override;
	void DrawDebug() override;
	void GenerateDepthMapFromLight() override;

	// プレイヤーの設定
	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

	void PushBuckLaserEnemyList(std::shared_ptr<Enemy> _enemy)
	{
		m_laserEnemyList.push_back(_enemy);
	}

	// 自分が当たるオブジェクトのタイプの設定
	void SetTargetColliderType(KdCollider::Type type);

	// 自分がオブジェクトと当たった時
	void Hit();

	void SetMotherEnemyPos(const Math::Vector3& _pos)
	{
		m_motherEnemyPos = _pos;
	}

private:
	void Rotation(Math::Vector3& _targetDir, float _clampAngMin, float _clampAngMax);
	void Collision();

private:
	KdGameObject* m_owner;
	float m_moveSpd;
	Math::Vector3 m_targetDir;
	Math::Vector3 m_motherEnemyPos;

	KdCollider::Type m_collisionType;

	std::weak_ptr<Player> m_wpPlayer;

	// レーザーの敵情報
	std::list<std::shared_ptr<Enemy>> m_laserEnemyList;

	bool m_debugFlg = false;
};