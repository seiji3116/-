#include "Player.h"
#include "../Camera/TPSCamera/TPSCamera.h"
#include "../Rock/Rock.h"
#include "../UI/GameUI/Reticle/Reticle.h"
#include "../UI/GameUI/ConcentratioLine/ConcentratioLine.h"

#include "../../../Scene/SceneManager.h"

void Player::Update()
{
	SetTarget();
	if (m_actionFlg)
	{
		Action();
	}
	GravityThrow();

	// ゲームオーバーになったらアクションをとめる
	if (m_gameOverFlg)
	{
		m_actionFlg = false;
		m_moveSpd -= 0.1f;
		if (m_moveSpd <= 0)
		{
			m_moveSpd = 0;
		}
	}
	//RayCollision(m_mWorld.Translation(), m_targetDir, 5, KdCollider::TypeEvent);
	//SphereCollision(m_mWorld.Translation(), 5, KdCollider::TypeEvent);
}

void Player::PostUpdate()
{
	Math::Matrix rotMatZ = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_roll));
	Math::Matrix rotMatX = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(m_pitch));

	Math::Matrix rotMat = rotMatX * rotMatZ;

	Math::Matrix transMat = Math::Matrix::CreateTranslation(m_moveVec * m_moveSpd);

	m_mWorld = rotMat * transMat * m_mWorld;
	m_moveDir = m_mWorld.Backward();
}

void Player::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawBright()
{
	//KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawLit()
{
	KdShaderManager::Instance().m_HD2DShader.DrawModel(*m_model, m_mWorld);
}

void Player::DrawDebug()
{
	m_pDebugWire->Draw();
}

void Player::Init()
{
	// 座標移動系初期化
	m_moveVec = Math::Vector3(0, 0, 0.1f);		// 移動量
	m_moveDir = Math::Vector3::Zero;
	m_moveSpd = 1.5f;

	// マトリックスの初期化
	Math::Matrix transMat = Math::Matrix::CreateTranslation({ 0,5,0 });
	m_mWorld = transMat;

	m_gravityCnt = 30.f;
	m_hitPoint = 5;

	// フラグ関係初期化
	m_keyFlg = false;
	m_gravityKeyFlg = false;
	m_gravityFlg = false;
	m_throwKeyFlg = false;
	m_accelFlg = false;
	m_actionFlg = true;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();

	// モデルの読込
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Player/Player3.gltf"));

	//当たり判定初期化
	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerModel", m_model, KdCollider::TypeDamage);
}

void Player::Hit()
{
	if (m_hitPoint >= 0)
	{
		m_hitPoint--;
		KdAudioManager::Instance().Play("Asset/Sounds/Game/PlayerDamage.wav", false, 0.3f);
	}
	if(m_hitPoint <= 0)
	{
		KdAudioManager::Instance().Play("Asset/Sounds/Game/Bom.wav", false, 0.5f);
		KdEffekseerManager::GetInstance().Play("ToonHit/ToonHit.efk", m_mWorld.Translation(), false);
		m_isExpired = true;
		m_gameOverFlg = true;
		std::shared_ptr<Reticle> spReticle = m_reticle.lock();
		spReticle->AliveOff();
	}
}

void Player::Action()
{
	// ローカル軸管理
	Math::Vector2 inputMove = KdInputManager::Instance().GetAxisState("move");
	m_roll -= inputMove.x * 0.1f;
	m_pitch -= inputMove.y * 0.1f;

	if (m_roll > 2)
	{
		m_roll = 2;
	}
	if (m_roll > 0)
	{
		m_roll -= 0.05f;
		if (m_roll < 0)
		{
			m_roll = 0;
		}
	}

	if (m_roll < -2)
	{
		m_roll = -2;
	}
	if (m_roll < 0)
	{
		m_roll += 0.05f;
		if (m_roll > 0)
		{
			m_roll = 0;
		}
	}

	if (m_pitch < -2)
	{
		m_pitch = -2;
	}
	if (m_pitch < 0)
	{
		m_pitch += 0.05f;
		if (m_pitch > 0)
		{
			m_pitch = 0;
		}
	}

	if (m_pitch > 2)
	{
		m_pitch = 2;
	}
	if (m_pitch > 0)
	{
		m_pitch -= 0.05f;
		if (m_pitch < 0)
		{
			m_pitch = 0;
		}
	}

	// 加速
	std::shared_ptr<ConcentratioLine> conLine = m_concentratioLine.lock();
	if (KdInputManager::Instance().IsPress("accel"))
	{
		if (!m_keyFlg)
		{
			m_keyFlg = true;
			if (m_accelFlg)
			{
				m_accelFlg = false;
			}
			else
			{
				m_accelFlg = true;
			}
			conLine->SetAccelFlg(m_accelFlg);
		}
	}
	else
	{
		m_keyFlg = false;
	}

	if (m_accelFlg)
	{
		m_moveSpd += 0.1f;
		if (m_moveSpd >= 4)
		{
			m_moveSpd = 4;
		}
	}
	else
	{
		m_moveSpd -= 0.1f;
		if (m_moveSpd <= 1.5f)
		{
			m_moveSpd = 1.5f;
		}
	}
}

bool Player::RayCollision(const Math::Vector3& _centerPos, const Math::Vector3& _dir, float _range, KdCollider::Type _type)
{
	//========================================================
	//レイ判定
	//========================================================
	// レイ判定用に変数を作成
	KdCollider::RayInfo rayInfo;

	// レイの発射座標を設定
	rayInfo.m_pos = _centerPos;

	// レイの方向を設定
	Math::Vector3 dir = _dir;
	dir.Normalize();
	rayInfo.m_dir = dir;

	// レイの長さを設定
	rayInfo.m_range = _range;

	// 当たり判定をしたいタイプを設定
	rayInfo.m_type = _type;

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
	bool hit = false;
	m_overlapDistance = 0.0f;
	for (auto& ret : retRayList)
	{
		// レイを遮断しオーバー↓長さが
		// 1番長いものを探す
		if (m_overlapDistance < ret.m_overlapDistance)
		{
			m_overlapDistance = ret.m_overlapDistance;
			m_hitPos = ret.m_hitPos;
			m_hitDir = ret.m_hitDir;
			hit = true;
		}
	}

	// 当たっていたら
	if (hit)
	{
		return true;
	}
	return false;
}

bool Player::SphereCollision(const Math::Vector3& _centerPos, const float _radius, KdCollider::Type _type)
{
	//========================================================
	//球判定
	//========================================================
	// 球判定用の変数
	KdCollider::SphereInfo sphereInfo;

	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = _centerPos;

	// 球の半径を設定
	sphereInfo.m_sphere.Radius = _radius;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = _type;

	// デバッグ用
	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

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
	bool hit = false;
	m_overlapDistance = 0.0f;
	for (auto& ret : retSphereList)
	{
		// １番近くで当たったものを探す
		if (m_overlapDistance < ret.m_overlapDistance)
		{
			m_overlapDistance = ret.m_overlapDistance;
			m_hitPos = ret.m_hitPos;
			m_hitDir = ret.m_hitDir;
			hit = true;
		}
	}
	if (hit)
	{
		// 球とモデルが当たっている
		return true;
	}
	// 弾と当たっていなかったらfalseで帰る
	return false;
}

void Player::GravityThrow()
{
	if (m_handle != 100000)
	{
		KdEffekseerManager::GetInstance().SetPos(m_handle, m_mWorld.Translation());
	}

	// 岩などの引き寄せ
	if (KdInputManager::Instance().IsPress("gravity"))
	{
		if (!m_gravityKeyFlg)
		{
			m_gravityKeyFlg = true;
			// リストにオブジェクトが1つもなく、重力操作可能状態だったら
			if (!m_gravityFlg && m_gravityList.size() <= 0)
			{
				KdAudioManager::Instance().Play("Asset/Sounds/Game/Gravity.wav", false, 0.3f);
				m_handle = KdEffekseerManager::GetInstance().Play("Gravity/Gravity.efk", m_mWorld.Translation(), false);
				m_gravityFlg = true;
			}
			else
			{
				if (m_gravityList.size() == 0)
				{
					m_gravityList.clear();
					m_gravityFlg = false;
					return;
				}
				// リストの最初のオブジェクトを投げる
				auto& it = m_gravityList.begin();
				KdAudioManager::Instance().Play("Asset/Sounds/Game/GravityThrow.wav", false, 0.5f);
				(*m_gravityList.begin())->ChengeThrow(m_targetDir);
				m_gravityList.erase(it);

			}
		}
	}
	else
	{
		m_gravityKeyFlg = false;
	}

	// 一定時間経過すると重力操作可能状態へ移行
	m_gravityCnt--;
	if (m_gravityCnt <= 0)
	{
		m_gravityFlg = false;
		m_gravityCnt = 30.f;
	}
}

void Player::SetTarget()
{
	// ターゲットにできるオブジェクトの中からターゲットするオブジェクトを指定
	const std::shared_ptr<CameraBase> spCamera = m_camera.lock();

	if (spCamera)
	{
		Math::Vector3 rayDir = Math::Vector3::Zero;
		float range = 100;

		// レティクルの2D座標を3D座標に変換
		std::shared_ptr<Reticle> spReticle = m_reticle.lock();
		POINT reticlePos = { spReticle->GetPos().x + 640, spReticle->GetPos().y + 360 };
		spCamera->WorkCamera()->GenerateRayInfoFromClientPos(reticlePos, spCamera->GetPos(), rayDir, range);

		m_pDebugWire->AddDebugLine(m_mWorld.Translation(), rayDir, range);

		// レイに当たったらあったところをターゲットとして指定
		if (RayCollision(m_mWorld.Translation(), rayDir, range, KdCollider::TypeTarget))
		{
			m_targetDir = m_hitDir;
			m_targetPos = m_hitPos;
		}
		// 当たってなかったら3Dに変換したベクトルを指定
		else
		{
			m_targetDir = rayDir;
			m_targetPos = m_targetDir * range;
		}
	}
}
