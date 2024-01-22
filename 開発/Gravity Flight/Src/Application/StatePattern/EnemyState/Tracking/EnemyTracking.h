#pragma once
#include "../../BaseState.h"

class EnemyTracking :public BaseState
{
public:
	void EnemyInit(Enemy& _owner)override;
	void EnemyUpdate(Enemy& _owner)override;
	void EnemyShot(Enemy& _owner)override;
	void EnemyAction(Enemy& _owner)override;
	void Hit(Enemy& _owner)override;

private:
	void Rotate(Math::Vector3 _targetDir);
};