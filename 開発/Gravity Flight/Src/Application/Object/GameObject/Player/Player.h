#pragma once

class BaseScene;
class Rock;
class Reticle;
class ConcentratioLine;

class Player : public KdGameObject
{
public:

	Player() { Init(); };
	~Player() {};

	void Update()override;
	void PostUpdate()override;

	void GenerateDepthMapFromLight()override;
	void DrawBright()override;
	void DrawLit()override;
	void DrawDebug() override;
	void Init()override;

	void SetOener(std::shared_ptr<BaseScene> _owner)
	{
		m_owner = _owner;
	}

	void SetReticle(std::shared_ptr<Reticle> _reticle)
	{
		m_reticle = _reticle;
	}

	void SetConcentratioLine(std::shared_ptr<ConcentratioLine> _concentratioLine)
	{
		m_concentratioLine = _concentratioLine;
	}

	void SetMoveSpd(const float& _spd)
	{
		m_moveSpd = _spd;
	}
	
	void SetActionFlg(bool _flg) { m_actionFlg = _flg; }

	bool GetGravityFlg()
	{
		return m_gravityFlg;
	}

	bool PushBackGravityList(KdGameObject* _gravityObj)
	{
		if (m_gravityList.size() < 3)
		{
			m_gravityList.push_back(_gravityObj);
			return true;
		}
		else
		{
			m_gravityFlg = false;
			return false;
		}
	}

	void PushBackTargetList(std::shared_ptr<KdGameObject> _targetObj)
	{
		m_targetList.push_back(_targetObj);
	}

	void Hit();

	bool GetGameOverFlg() { return m_gameOverFlg; }

	const bool& GetGameClearFlg() { return m_gameClearFlg; }
	void SetGameClearFlg(bool _flg) { m_gameClearFlg = _flg; }

private:
	void Action();

	// 当たり判定関係
	bool RayCollision(const Math::Vector3& _centerPos, const Math::Vector3& _dir, float _range,KdCollider::Type _type);
	bool SphereCollision(const Math::Vector3& _centerPos, const float _radius, KdCollider::Type _type);

	void GravityThrow();
	void SetTarget();

private:

	std::shared_ptr<KdModelWork> m_model;	// モデル情報
	float m_roll = 0;						// z軸用回転量
	float m_pitch = 0;						// x軸用回転量

	std::weak_ptr<BaseScene> m_owner;

	// 座標移動系
	Math::Vector3 m_moveVec;				// 移動量
	Math::Vector3 m_moveDir;				// 進みたい角度
	float m_moveSpd;						// 移動する速度
	bool m_accelFlg;						// 加速フラグ

	// 岩等の引き寄せ処理用
	bool m_gravityFlg;
	float m_gravityCnt;
	std::list<KdGameObject*> m_gravityList;
	std::list<std::shared_ptr<KdGameObject>> m_targetList;
	bool m_actionFlg;

	// プレイヤーのステータス
	float m_hitPoint;

	// ターゲット判定用
	std::weak_ptr<Reticle> m_reticle;

	// 集中線
	std::weak_ptr<ConcentratioLine> m_concentratioLine;

	// 当たり判定用
	Math::Vector3 m_hitPos;			// 衝突した座標
	Math::Vector3 m_hitDir;			// 対象からの方向ベクトル（押し返しなどに使う
	float m_overlapDistance = 0.0f; // 重なり量

	// フラグ関係
	bool m_keyFlg;
	bool m_gravityKeyFlg;
	bool m_throwKeyFlg;
	bool m_gameOverFlg = false;
	bool m_gameClearFlg = false;

	// エフェクト
	Effekseer::Handle m_handle = 100000;
};