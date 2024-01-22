#pragma once

class Player;
class Enemy;

class Rock : public KdGameObject
{
public:

	Rock() { Init(); };
	~Rock() {};

	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void DrawBright()override;
	void DrawDebug() override;
	void Init()override;
	void Update()override;

	void SetOwnerPos(const Math::Vector3& _pos) { m_ownerPos = _pos; }
	void SetRevolutionPos(const Math::Vector3& _pos) { m_revolutionPos = _pos; }
	void SetPlayer(std::shared_ptr<Player> _player)
	{
		m_wpPlayer = _player;
	}

	void PushBuckLaserEnemyList(std::shared_ptr<Enemy> _enemy)
	{
		m_laserEnemyList.push_back(_enemy);
	}

private:
	void Throw();
	void Gravity();
	void Collishon();
	void Rotate(Math::Vector3 _targetDir);

private:

	std::shared_ptr<KdModelWork> m_model;

	std::weak_ptr<Player> m_wpPlayer;

	Math::Vector3 m_scale;
	Math::Matrix m_transMat = Math::Matrix::Identity;
	Math::Matrix m_rotateMat = Math::Matrix::Identity;
	Math::Matrix m_scaleMat = Math::Matrix::Identity;
	Math::Matrix m_revolutionMat = Math::Matrix::Identity;
	float m_angle;
	Math::Vector3 m_ownerPos;
	Math::Vector3 m_revolutionPos;

	// �G�t�F�N�g
	Effekseer::Handle m_handle = 100000;

	// �v���C���[�Ɉ����񂹂���Ƃ�
	Math::Vector3 m_playerVec;		// �v���C���[�ւ̃x�N�g��
	Math::Vector3 m_targetVec;		// �^�[�Q�b�g�̕���
	Math::Vector3 m_targetPos;		// �^�[�Q�b�g�̍��W
	Math::Vector3 m_localPos;		// �^�[�Q�b�g����ǂ̂��炢����Ă��邩

	// ���[�U�[�̓G���
	std::list<std::shared_ptr<Enemy>> m_laserEnemyList;

	bool m_debugFlg = false;
};