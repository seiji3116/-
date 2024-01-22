#pragma once

class KdDebugWireFrame;
class CameraBase;

// ゲーム上に存在するすべてのオブジェクトの基底となるクラス
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:

	// どのような描画を行うのかを設定するTypeID：Bitフラグで複数指定可能
	enum
	{
		eDrawTypeLit = 1 << 0,
		eDrawTypeUnLit = 1 << 1,
		eDrawTypeBright = 1 << 2,
		eDrawTypeUI = 1 << 3,
		eDrawTypeDepthOfShadow = 1 << 4,
	};

	KdGameObject() {}
	virtual ~KdGameObject() { Release(); }

	// 生成される全てに共通するパラメータに対する初期化のみ
	virtual void Init() {}

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}

	// それぞれの状況で描画する関数
	virtual void PreDraw();
	virtual void GenerateDepthMapFromLight() {}
	virtual void DrawLit() {}
	virtual void DrawUnLit() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}
	virtual void DrawDebug();

	virtual void SetAsset(const std::string&) {}

	virtual void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	virtual Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// 拡大率を変更する関数
	void SetScale(float scalar);
	virtual void SetScale(const Math::Vector3& scale);
	virtual Math::Vector3 GetScale() const;

	const Math::Matrix& GetMatrix() const { return m_mWorld; }
	void SetMatrix(const Math::Matrix& _mWorld) { m_mWorld = _mWorld; }

	// add: 
	const std::weak_ptr<CameraBase>& GetCamera() const { return m_camera; }

	// add: モデルをセットする関数
	void SetModel(const std::shared_ptr<KdModelWork>& _model) 
	{
		m_model = std::make_shared<KdModelWork>();
		m_model = _model;
	}

	// add: コライダーをセット
	void SetCollision(std::string_view name, const Math::Vector3& localPos, float radius, UINT type)
	{
		m_pCollider = std::make_unique<KdCollider>();
		m_pCollider->RegisterCollisionShape(name, localPos, radius, type);
	}

	const std::shared_ptr<KdModelWork>& GetModel() const
	{
		return m_model;
	}

	// add:
	virtual Math::Vector3 GetTargetDir() { return m_targetDir; }
	virtual Math::Vector3 GetTargetPos() { return m_targetPos; }

	void SetCamera(const std::shared_ptr<CameraBase> _camera)
	{
		m_camera = _camera;
	}

	void Hit() { m_isExpired = true; }

	virtual bool IsExpired() const { return m_isExpired; }
	virtual void IsExpired(bool _expired) { m_isExpired = _expired; }

	virtual bool IsVisible() const { return false; }

	// 視錐台範囲内に入っているかどうか
	virtual bool CheckInScreen(const DirectX::BoundingFrustum&) const { return false; }

	// カメラからの距離を計算
	virtual void CalcDistSqrFromCamera(const Math::Vector3& camPos);

	float GetDistSqrFromCamera() const { return m_distSqrFromCamera; }

	UINT GetDrawType() const { return m_drawType; }

	bool Intersects(const KdCollider::SphereInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);
	bool Intersects(const KdCollider::RayInfo& targetShape, std::list<KdCollider::CollisionResult>* pResults);

	// add: 重力操作関係
	void ChengeThrow(Math::Vector3 _targetVec)
	{
		m_targetDir = _targetVec;
		m_gravityFlg = false;
		m_throwFlg = true;
	}

	// add: 上限と下限を入れるとランダムな値を返す
	int Random(int _min, int _max)
	{
		std::mt19937 engine{ std::random_device{}() };
		std::uniform_int_distribution<int> dist(_min, _max);
		auto n = dist(engine);
		return n;
	}

	void SinCurveAlpha(float* _num, float _angleSpeed)
	{
		m_sinAngle += _angleSpeed;

		if (m_sinAngle >= 200)
		{
			m_sinAngle -= 200;
		}

		*_num = (sin(DirectX::XMConvertToRadians(m_sinAngle)) + 1) / 2;
	}

	void SinCurve(float* _num, float _angleSpeed)
	{
		m_sinAngle += _angleSpeed;

		*_num = (sin(DirectX::XMConvertToRadians(m_sinAngle)) + 1) / 2;
	}

	bool GetInvincibleFlg() { return m_invincibleFlg; }
	float GetInvincibleCnt() { return m_invincibleCnt; }
	void SetInvincibleFlg(const bool _flg) { m_invincibleFlg = _flg; }
	void SetInvincibleCnt(const float _num) { m_invincibleCnt = _num; }

protected:

	void Release() {}

	// 描画タイプ・何の描画を行うのかを決める / 最適な描画リスト作成用
	UINT m_drawType = 0;

	// カメラからの距離
	float m_distSqrFromCamera = 0;

	// 存在消滅フラグ
	bool m_isExpired = false;

	// 3D空間に存在する機能
	Math::Matrix	m_mWorld;

	// add: 
	Math::Vector3	m_targetDir;
	Math::Vector3	m_targetPos;

	std::weak_ptr<CameraBase> m_camera;		// カメラ情報

	// モデル用
	std::shared_ptr<KdModelWork> m_model;

	// 当たり判定クラス
	std::unique_ptr<KdCollider> m_pCollider = nullptr;

	//デバッグ用
	std::unique_ptr<KdDebugWireFrame> m_pDebugWire = nullptr;

	// add: 重力操作感系
	bool	m_gravityFlg = false;			// 今引き寄せられているか
	bool	m_throwFlg = false;				// 今投げられているか
	float	m_gravityPow = 0;				// 引き寄せられるspeed

	float	m_sinAngle = 0;

	float m_invincibleCnt = 20.f;
	bool m_invincibleFlg = false;
};