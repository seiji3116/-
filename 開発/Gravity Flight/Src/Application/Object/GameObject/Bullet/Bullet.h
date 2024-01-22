#pragma once

class Bullet : public KdGameObject
{
public:

	Bullet() { Init(); };
	~Bullet() {};

	// 描画関数
	void GenerateDepthMapFromLight()override;
	void DrawLit()override;
	void DrawBright()override;
	
	// 初期化関数
	void Init()override;

	// 更新関数
	void Update()override;
	void PostUpdate()override;

	// セッター
	void SetPos(Math::Vector3 _pos)
	{
		m_pos = _pos;
	}
	void SetMoveDir(Math::Vector3 _dir)
	{
		m_moveDir = _dir;
		m_moveDir.Normalize();
	}

private:
	void UpdateRotate();

private:
	std::shared_ptr<KdModelWork> m_model;

	// マトリックス
	Math::Matrix m_transMat;
	Math::Matrix m_rotateMat;

	// 座標移動系
	Math::Vector3 m_pos;
	Math::Vector3 m_moveDir;
	float m_moveSpd;
};