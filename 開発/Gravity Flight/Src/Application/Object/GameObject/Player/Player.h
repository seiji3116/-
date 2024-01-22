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

	// �����蔻��֌W
	bool RayCollision(const Math::Vector3& _centerPos, const Math::Vector3& _dir, float _range,KdCollider::Type _type);
	bool SphereCollision(const Math::Vector3& _centerPos, const float _radius, KdCollider::Type _type);

	void GravityThrow();
	void SetTarget();

private:

	std::shared_ptr<KdModelWork> m_model;	// ���f�����
	float m_roll = 0;						// z���p��]��
	float m_pitch = 0;						// x���p��]��

	std::weak_ptr<BaseScene> m_owner;

	// ���W�ړ��n
	Math::Vector3 m_moveVec;				// �ړ���
	Math::Vector3 m_moveDir;				// �i�݂����p�x
	float m_moveSpd;						// �ړ����鑬�x
	bool m_accelFlg;						// �����t���O

	// �ⓙ�̈����񂹏����p
	bool m_gravityFlg;
	float m_gravityCnt;
	std::list<KdGameObject*> m_gravityList;
	std::list<std::shared_ptr<KdGameObject>> m_targetList;
	bool m_actionFlg;

	// �v���C���[�̃X�e�[�^�X
	float m_hitPoint;

	// �^�[�Q�b�g����p
	std::weak_ptr<Reticle> m_reticle;

	// �W����
	std::weak_ptr<ConcentratioLine> m_concentratioLine;

	// �����蔻��p
	Math::Vector3 m_hitPos;			// �Փ˂������W
	Math::Vector3 m_hitDir;			// �Ώۂ���̕����x�N�g���i�����Ԃ��ȂǂɎg��
	float m_overlapDistance = 0.0f; // �d�Ȃ��

	// �t���O�֌W
	bool m_keyFlg;
	bool m_gravityKeyFlg;
	bool m_throwKeyFlg;
	bool m_gameOverFlg = false;
	bool m_gameClearFlg = false;

	// �G�t�F�N�g
	Effekseer::Handle m_handle = 100000;
};