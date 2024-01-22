#pragma once

class BaseState;
class Enemy;

class StateManager
{
public:
	// �X�e�[�g�̏����l�ݒ�
	StateManager(){}

public:
	// Base�̊֐��Ăяo���p
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
	// �X�e�[�g�̐؂�ւ�
	void EnemyChengeLaser(Enemy& _owner);		// ���[�U�[�̔���
	void EnemyChengeNormal(Enemy& _owner);		// �ʏ�e�𔭎˂ł�����
	void EnemyChengeTracking(Enemy& _owner);	// �v���C���[�ւ̒ǔ����

private:
	// �����Ő؂�ւ����s��
	// ���݂̃X�e�[�g���폜���A���̃X�e�[�g�Ɉڍs����
	void EnemyChengeState(std::shared_ptr<BaseState> state, Enemy& _owner);

private:
	// ���݂̃X�e�[�g
	std::shared_ptr<BaseState> m_nowState;
};