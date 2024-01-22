#include "EnemyLaser.h"
#include "../../../Object/GameObject/Laser/Laser.h"
#include "../../../Object/GameObject/Effect/ShieldEffect/ShieldEffect.h"
#include "../../../Object/GameObject/Enemy/Enemy.h"
#include "../../../Scene/SceneManager.h"

void EnemyLaser::EnemyInit(Enemy& _owner)
{
	m_model = std::make_shared<KdModelWork>();
	m_model->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Enemy/LaserEnemy.gltf"));

	//当たり判定初期化
	m_pCollider = std::make_unique<KdCollider>();

	// オーナーに情報をセット
	_owner.SetModel(m_model);
	_owner.SetCollision("LaserEnemy", m_mWorld.Translation(), 5.f, KdCollider::TypeEvent);

	m_laserFlg = false;

	// シールド関係
	m_shieldCnt = 1;
	m_shieldFlg = true;

	m_debugColor = Math::Color(1, 1, 1);
}

void EnemyLaser::EnemyUpdate(Enemy& _owner)
{
	_owner.SetDebugSphere(m_mWorld.Translation(), 5.f, m_debugColor);

	if (!m_laserFlg)
	{
		// レーザー生成
		m_laserFlg = true;
		m_laser = std::make_shared<Laser>();
		m_laser->SetDir(_owner.GetTargetDir());
		m_laser->SetStartPos(m_mWorld.Translation() + (m_mWorld.Backward() * 5));
		m_laser->SetRange(Math::Vector2(3, 20));
		m_laser->SetCamera(_owner.GetCamera().lock());
		SceneManager::Instance().AddObject(m_laser);

		// シールド生成
		m_shield = std::make_shared<ShieldEffect>();
		m_shield->SetPos(m_mWorld.Translation());
		m_shieldNum = _owner.GetShieldNum();
		std::string shieldNum = "Shield" + std::to_string(m_shieldNum);
		m_shield->SetEffectName(shieldNum);
		SceneManager::Instance().AddObject(m_shield);
	}

	m_laser->SetDir(_owner.GetTargetDir());
	m_laser->SetStartPos(m_mWorld.Translation() + (m_mWorld.Backward() * 5));
	Rotate(_owner.GetTargetDir());

	float cnt = _owner.GetInvincibleCnt();
	if (cnt <= 0)
	{
		_owner.SetInvincibleFlg(false);
	}
	if (cnt >= 0)
	{
		cnt--;
		_owner.SetInvincibleCnt(cnt);
	}

	_owner.SetMatrix(m_mWorld);
}

void EnemyLaser::EnemyShot(Enemy& _owner)
{
}

void EnemyLaser::EnemyAction(Enemy& _owner)
{
}

void EnemyLaser::Hit(Enemy& _owner)
{
	m_shieldCnt--;
	m_debugColor = Math::Color(1, 0, 0);

	if (!_owner.GetInvincibleFlg())
	{
		if (m_shieldCnt <= 0 && !m_shieldFlg)
		{
			_owner.IsExpired(true);
			m_laser->IsExpired(true);
			if (m_handle == 100000)
			{
				KdAudioManager::Instance().Play("Asset/Sounds/Game/Bom.wav", false, 0.5f);
				m_handle = KdEffekseerManager::GetInstance().Play("ToonHit/ToonHit.efk", m_mWorld.Translation(), false);
				KdEffekseerManager::GetInstance().SetScale(m_handle, 2.f);
			}
		}
		if (m_shieldCnt <= 0 && m_shieldFlg)
		{
			KdAudioManager::Instance().Play("Asset/Sounds/Game/ShieldBreak.wav",false, 0.5f);
			m_shield->StopEffect();
			_owner.SetShieldFlg(false);
			m_shieldFlg = false;
			/*m_shield->IsExpired(true);*/
		}
	}

	_owner.SetInvincibleFlg(true);
	_owner.SetInvincibleCnt(20.f);
}

void EnemyLaser::Rotate(Math::Vector3 _targetDir)
{
	Math::Vector3 nowDir = m_mWorld.Backward();
	nowDir.Normalize();
	_targetDir.Normalize();

	float dot = nowDir.Dot(_targetDir);
	dot = std::clamp(dot, -1.f, 1.f);

	float betweeanAng = acos(dot);
	betweeanAng = DirectX::XMConvertToDegrees(betweeanAng);

	float rotateAng = std::clamp(betweeanAng, -4.f, 4.f);

	Math::Vector3 rotAxis;
	nowDir.Cross(_targetDir, rotAxis);
	if (rotAxis.LengthSquared() == 0)return;

	Math::Matrix rotation;
	rotation = Math::Matrix::CreateFromAxisAngle(rotAxis, DirectX::XMConvertToRadians(rotateAng));

	Math::Vector3 pos = m_mWorld.Translation();
	m_mWorld.Translation(Math::Vector3(0, 0, 0));

	m_mWorld *= rotation;
	m_mWorld.Translation(pos);
}

void EnemyLaser::Collishion()
{
	////========================================================
	////球判定
	////========================================================
	//// 球判定用の変数
	//KdCollider::SphereInfo sphereInfo;

	//// 球の中心位置を設定
	//sphereInfo.m_sphere.Center = m_mWorld.Translation();

	//// 球の半径を設定
	//sphereInfo.m_sphere.Radius = m_shieldRadius;

	//// 当たり判定をしたいタイプを設定
	//sphereInfo.m_type = KdCollider::TypePlayerShot;

	//// デバッグ用
	////m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius);

	//// 当たったオブジェクト情報を格納するリスト
	//std::list<KdCollider::CollisionResult> retSphereList;

	//// 球と当たり判定をする
	//// マップとの当たり判定
	//for (auto& obj : SceneManager::Instance().GetObjList())
	//{
	//	obj->Intersects
	//	(
	//		sphereInfo,
	//		&retSphereList
	//	);
	//}

	//// 当たったリスト情報から1番近いオブジェクトを検出
	//bool Hit = false;
	//float overlapDistance = 0.0f;
	//for (auto& ret : retSphereList)
	//{
	//	// １番近くで当たったものを探す
	//	if (overlapDistance < ret.m_overlapDistance)
	//	{
	//		overlapDistance = ret.m_overlapDistance;
	//		Hit = true;
	//	}
	//}
	//if (Hit)
	//{
	//	// 球とモデルが当たっている
	//	return true;
	//}
	//// 弾と当たっていなかったらfalseで帰る
	//return false;
}
