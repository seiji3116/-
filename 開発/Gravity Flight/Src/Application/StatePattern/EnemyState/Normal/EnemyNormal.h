#pragma once
#include "../../BaseState.h"

class EnemyNormal :public BaseState
{
public:
	void EnemyInit(Enemy& _owner)override;
	void EnemyUpdate(Enemy& _owner)override;
	void EnemyShot(Enemy& _owner)override;
	void EnemyAction(Enemy& _owner)override;
	void Hit(Enemy& _owner)override;

private:
	void Rotate();
	void UpdatePos(const Math::Vector3& _targetPos);
	int Random(int _min, int _max);

	const Math::Matrix GetRotationMatrix() const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_DegAng.y),
			DirectX::XMConvertToRadians(m_DegAng.x),
			DirectX::XMConvertToRadians(m_DegAng.z));
	}

private:

	// çÏã∆ópçsóÒ
	static const int GHOST_MAX = 60;
	Math::Vector3 m_ghostTargetPos[GHOST_MAX];

	float m_shotCnt;
	float m_targetDistance;

	Math::Matrix m_localTransMat;
	Math::Matrix m_rotateMat;
	Math::Matrix m_transMat;

	Math::Vector3 m_DegAng;
	Math::Vector3 m_addAng;
};