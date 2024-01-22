#pragma once

class Enemy;

class BaseState
{
public:
	// 基本関数
	virtual void EnemyInit(Enemy& _owner) = 0;
	virtual void EnemyUpdate(Enemy& _owner) = 0;
	virtual void EnemyShot(Enemy& _owner) = 0;			// 弾の発射
	virtual void EnemyAction(Enemy& _owner) = 0;		// 挙動の大本
	virtual void Hit(Enemy& _owner) = 0;				// 弾が当たった時

public:
	// ゲッター
	Math::Vector3 GetMoveDir() const { return m_moveDir; }		// 移動ベクトル取得
	float GetActionCnt() const { return m_actionCnt; }			// 現在のAction実行時間の取得
	bool GetActionEndFlg() const { return m_actionEndFlg; }		// 現在のActiongが終わっているかどうか
	bool GetShieldFlg() const { return m_shieldFlg; }			// シールド生存フラグ

public:
	// セッター
	void SetMoveDir(const Math::Vector3& _moveDir) { m_moveDir = _moveDir; }
	void SetMatrix(const Math::Matrix& _matrix) { m_mWorld = _matrix; }
	void SetPos(const Math::Vector3& _pos) { m_mWorld.Translation(_pos); }

protected:
	std::shared_ptr<KdModelWork> m_model = nullptr;				// モデルデータ

	Math::Matrix m_mWorld;										// ワールド行列

	std::unique_ptr<KdCollider> m_pCollider = nullptr;			// 当たり判定クラス

	Math::Vector3 m_moveDir = Math::Vector3::Zero;				// 移動ベクトル
	float m_moveSpd = 0;										// 移動speed

	float m_standCnt = 0;										// 各挙動をするときの待機時間
	float m_actionCnt = 1;										// 各挙動を実行しているときの時間
	bool m_actionEndFlg = false;								// 挙動が終わっていてステートを変えてもいいかを見るフラグ

	bool m_shieldFlg = false;
};