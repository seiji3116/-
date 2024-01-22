#include "TrackingMissile.h"
#include "../../../Object/GameObject/Player/Player.h"
#include "../../../Scene/SceneManager.h"

void TrackingMissile::Init()
{
	// モデル読込
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Missile/TrackingMissile.gltf"));

	// 移動量
	m_moveSpd = 0.7f;
	m_targetDir = Math::Vector3::Zero;

	// デバッグワイヤー初期化
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// どのタイプに当たるかの設定
	m_collisionType = KdCollider::TypeDamage;

	//当たり判定初期化
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("TrackingMissile", m_mWorld.Translation(), 5.f, KdCollider::TypeEnemyShot);
}

void TrackingMissile::Update()
{
	//m_pDebugWire->AddDebugSphere(m_mWorld.Translation(), 5.f);

	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (!spPlayer) { return; }

	if (spPlayer->GetGameClearFlg())
	{
		m_isExpired = true;

	}

	float playerLength = (spPlayer->GetPos() - m_mWorld.Translation()).Length();
	if (playerLength >= 200)
	{
		Hit();
	}

	// 座標移動
	if (m_targetDir == Math::Vector3::Zero)
	{
		m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();
		// 初回だけ正面をプレイヤーに完全に合わせる
		Rotation(m_targetDir, -1000.f, 1000.f);
	}
	m_targetDir = spPlayer->GetPos() - m_mWorld.Translation();

	Math::Vector3 vMove = m_mWorld.Backward();
	vMove.Normalize();

	vMove *= m_moveSpd;
	Math::Matrix trans = Math::Matrix::CreateTranslation(vMove);
	m_mWorld *= trans;

	Rotation(m_targetDir, -0.025f, 0.025f);	// ターゲットへの追尾処理
	Collision();							// 当たり判定

}

void TrackingMissile::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TrackingMissile::DrawDebug()
{
	m_pDebugWire->Draw();
}

void TrackingMissile::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void TrackingMissile::SetTargetColliderType(KdCollider::Type type)
{
	m_collisionType = type;
}

void TrackingMissile::Hit()
{
	m_isExpired = true;
}

void TrackingMissile::Rotation(Math::Vector3& _targetDir, float _clampAngMin, float _clampAngMax)
{
	Math::Vector3 nowDir = m_mWorld.Backward();
	nowDir.Normalize();
	_targetDir.Normalize();

	float dot = nowDir.Dot(_targetDir);
	dot = std::clamp(dot, -1.f, 1.f);

	float betweeanAng = acos(dot);
	betweeanAng = DirectX::XMConvertToDegrees(betweeanAng);

	float rotateAng = std::clamp(betweeanAng, _clampAngMin, _clampAngMax);

	Math::Vector3 rotAxis;
	nowDir.Cross(_targetDir, rotAxis);
	if (rotAxis.LengthSquared() == 0)return;

	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = GetPos();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}

void TrackingMissile::Collision()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (!spPlayer) { return; }
	if (spPlayer->GetGameClearFlg()) { return; }

	//========================================================
	//レイ判定
	//========================================================
	// レイ判定用に変数を作成
	KdCollider::RayInfo rayInfo;

	// レイの発射座標を設定
	rayInfo.m_pos = m_mWorld.Translation();

	// レイの方向を設定
	rayInfo.m_dir = m_mWorld.Backward();

	// レイの長さを設定
	rayInfo.m_range = m_moveSpd;

	// 当たり判定をしたいタイプを設定
	rayInfo.m_type = m_collisionType;

	// レイに当たったオブジェクト情報
	std::list<KdCollider::CollisionResult> retRayList;

	// レイと当たり判定をする
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			rayInfo,
			&retRayList
		);
	}

	// レイに当たったリストから1番近いオブジェクトを検出
	float maxOverLap = 0;
	Math::Vector3 groundPos;
	bool hit = false;
	for (auto& ret : retRayList)
	{
		// レイを遮断しオーバー↓長さが
		// 1番長いものを探す
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			groundPos = ret.m_hitPos;
			hit = true;
		}
	}

	// 当たっていたら
	if (hit)
	{
		Hit();
		spPlayer->Hit();
	}

	//========================================================
	//球判定
	//========================================================
	// 球判定用の変数
	KdCollider::SphereInfo sphereInfo;

	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = m_mWorld.Translation();

	// 球の半径を設定
	sphereInfo.m_sphere.Radius = 1.5f;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = m_collisionType;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	// 当たったオブジェクト情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 球と当たり判定をする
	// マップとの当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects
		(
			sphereInfo,
			&retSphereList
		);
	}

	// 当たったリスト情報から1番近いオブジェクトを検出
	for (auto& ret : retSphereList)
	{
		hit = true;
	}
	if (hit)
	{
		// 球とモデルが当たっている
		Hit();
		spPlayer->Hit();
	}
}