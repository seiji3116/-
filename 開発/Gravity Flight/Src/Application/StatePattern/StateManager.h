#pragma once

class BaseState;
class Enemy;

class StateManager
{
public:
	// ステートの初期値設定
	StateManager(){}

public:
	// Baseの関数呼び出し用
	void EnemyUpdate(Enemy& _owner);

	Math::Vector3 GetMoveDir() const;
	float GetActionCnt() const;
	bool GetActionEndFlg() const;
	bool GetShieldFlg() const;

	void SetMoveDir(const Math::Vector3& _moveDir);
	void SetMatrix(const Math::Matrix& _matrix);
	void SetPos(const Math::Vector3& _pos);

	void Hit(Enemy& _owner);

public:
	// ステートの切り替え
	void EnemyChengeLaser(Enemy& _owner);		// レーザーの発射
	void EnemyChengeNormal(Enemy& _owner);		// 通常弾を発射できる状態
	void EnemyChengeTracking(Enemy& _owner);	// プレイヤーへの追尾状態

private:
	// ここで切り替えを行う
	// 現在のステートを削除し、次のステートに移行する
	void EnemyChengeState(std::shared_ptr<BaseState> state, Enemy& _owner);

private:
	// 現在のステート
	std::shared_ptr<BaseState> m_nowState;
};