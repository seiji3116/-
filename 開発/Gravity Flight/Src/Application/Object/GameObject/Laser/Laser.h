#pragma once

class CameraBase;

class Laser :public KdGameObject
{
public:
	Laser() { Init(); }
	~Laser(){}

	// 基本関数
	void DrawBright()override;
	void DrawLit()override;
	void Init()override;
	void Update()override;
	void PostUpdate()override;

public:
	void SetDir(const Math::Vector3& _dir) { m_dir = _dir; }
	void SetRange(const Math::Vector2& _range) { m_range = _range; }
	void SetRange(const float _xRange, const float _yRange) { m_range = Math::Vector2(_xRange, _yRange); }
	void SetStartPos(const Math::Vector3& _pos)
	{
		// 発射位置のセット
		m_startPos = _pos;
		// 発射位置、方向ベクトル、長さから中心座標のセット
		if (m_range != Math::Vector2::Zero && m_dir != Math::Vector3::Zero)
		{
			float xHalfRange = m_range.y / 2;
			m_dir.Normalize();
			m_centerPos = (m_dir * xHalfRange) + m_startPos;
		}
	}

	void SetCamera(const std::shared_ptr<CameraBase> camera)
	{
		m_camera = camera;
	}

private:
	void Rotate(Math::Vector3 _dir);
	void UpdateCenterPos();

private:
	std::shared_ptr<KdSquarePolygon> m_poly;
	Math::Vector3 m_centerPos;
	Math::Vector3 m_startPos;
	Math::Vector3 m_dir;
	Math::Vector2 m_range;

	std::weak_ptr<CameraBase> m_camera;		// カメラ情報
};