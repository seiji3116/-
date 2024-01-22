#pragma once
#include "../../BaseState.h"

class ShieldEffect;
class Laser;
class Enemy;

class EnemyLaser :public BaseState
{
public:
	void EnemyInit(Enemy& _owner)override;
	void EnemyUpdate(Enemy& _owner)override;
	void EnemyShot(Enemy& _owner)override;
	void EnemyAction(Enemy& _owner)override;
	void Hit(Enemy& _owner)override;

private:
	void Rotate(Math::Vector3 _targetDir);
	void Collishion();

private:
	std::shared_ptr<Laser> m_laser;
	bool m_laserFlg;

	// �V�[���h�֌W
	std::shared_ptr<ShieldEffect> m_shield;
	int m_shieldCnt;							// �v���C���[�����˂����I�u�W�F�N�g�����񓖂�������
	float m_shieldRadius;
	int m_shieldNum;
	bool m_shieldFlg;

	// �G�t�F�N�g
	Effekseer::Handle m_handle = 100000;

	// �f�o�b�O
	Math::Color m_debugColor;
};