#pragma once

class Enemy;

class BaseState
{
public:
	// ��{�֐�
	virtual void EnemyInit(Enemy& _owner) = 0;
	virtual void EnemyUpdate(Enemy& _owner) = 0;
	virtual void EnemyShot(Enemy& _owner) = 0;			// �e�̔���
	virtual void EnemyAction(Enemy& _owner) = 0;		// �����̑�{
	virtual void Hit(Enemy& _owner) = 0;				// �e������������

public:
	// �Q�b�^�[
	Math::Vector3 GetMoveDir() const { return m_moveDir; }		// �ړ��x�N�g���擾
	float GetActionCnt() const { return m_actionCnt; }			// ���݂�Action���s���Ԃ̎擾
	bool GetActionEndFlg() const { return m_actionEndFlg; }		// ���݂�Actiong���I����Ă��邩�ǂ���
	bool GetShieldFlg() const { return m_shieldFlg; }			// �V�[���h�����t���O

public:
	// �Z�b�^�[
	void SetMoveDir(const Math::Vector3& _moveDir) { m_moveDir = _moveDir; }
	void SetMatrix(const Math::Matrix& _matrix) { m_mWorld = _matrix; }
	void SetPos(const Math::Vector3& _pos) { m_mWorld.Translation(_pos); }

protected:
	std::shared_ptr<KdModelWork> m_model = nullptr;				// ���f���f�[�^

	Math::Matrix m_mWorld;										// ���[���h�s��

	std::unique_ptr<KdCollider> m_pCollider = nullptr;			// �����蔻��N���X

	Math::Vector3 m_moveDir = Math::Vector3::Zero;				// �ړ��x�N�g��
	float m_moveSpd = 0;										// �ړ�speed

	float m_standCnt = 0;										// �e����������Ƃ��̑ҋ@����
	float m_actionCnt = 1;										// �e���������s���Ă���Ƃ��̎���
	bool m_actionEndFlg = false;								// �������I����Ă��ăX�e�[�g��ς��Ă�������������t���O

	bool m_shieldFlg = false;
};