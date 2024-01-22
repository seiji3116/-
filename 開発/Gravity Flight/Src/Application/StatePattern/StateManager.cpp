#include "StateManager.h"
#include "EnemyState/Normal/EnemyNormal.h"
#include "EnemyState/Laser/EnemyLaser.h"
#include "EnemyState/Tracking/EnemyTracking.h"
#include "../Object/GameObject/Enemy/Enemy.h"

void StateManager::EnemyUpdate(Enemy& _owner)
{
	m_nowState->EnemyUpdate(_owner);
}

Math::Vector3 StateManager::GetMoveDir() const
{
	return m_nowState->GetMoveDir();
}

float StateManager::GetActionCnt() const
{
	return m_nowState->GetActionCnt();
}

bool StateManager::GetActionEndFlg() const
{
	return m_nowState->GetActionEndFlg();
}

bool StateManager::GetShieldFlg() const
{
	return m_nowState->GetShieldFlg();
}

void StateManager::SetMoveDir(const Math::Vector3& _moveDir)
{
	m_nowState->SetMoveDir(_moveDir);
}

void StateManager::SetMatrix(const Math::Matrix& _matrix)
{
	m_nowState->SetMatrix(_matrix);
}

void StateManager::SetPos(const Math::Vector3& _pos)
{
	m_nowState->SetPos(_pos);
}

void StateManager::Hit(Enemy& _owner)
{
	m_nowState->Hit(_owner);
}

void StateManager::EnemyChengeLaser(Enemy& _owner)
{
	std::shared_ptr<EnemyLaser> state = std::make_shared<EnemyLaser>();
	EnemyChengeState(state, _owner);
}

void StateManager::EnemyChengeNormal(Enemy& _owner)
{
	std::shared_ptr<EnemyNormal> state = std::make_shared<EnemyNormal>();
	EnemyChengeState(state, _owner);
}

void StateManager::EnemyChengeTracking(Enemy& _owner)
{
	std::shared_ptr<EnemyTracking> state = std::make_shared<EnemyTracking>();
	EnemyChengeState(state, _owner);
}

void StateManager::EnemyChengeState(std::shared_ptr<BaseState> state, Enemy& _owner)
{
	if (m_nowState == state)return;		// 次のステートと前のステートが同じ場合切り替えを行わない

	m_nowState.reset();					// 前までのステートを削除する
	m_nowState = state;					// 指定したステートに切り替える
	m_nowState->EnemyInit(_owner);			// 切り替えと同時に初期化し、オーナーのセット
}
