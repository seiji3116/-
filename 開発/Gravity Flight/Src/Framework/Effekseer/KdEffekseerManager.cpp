#include"KdEffekseerManager.h"
#include "Application/Object/GameObject/Camera/CameraBase.h"

void KdEffekseerManager::Create(int w, int h)
{
	// エフェクトのレンダラーの作成
	m_efkRenderer = ::EffekseerRendererDX11::Renderer::Create(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext(), 8000);

	// エフェクトのマネージャーの作成
	m_efkManager = ::Effekseer::Manager::Create(8000);

	// 左手座標系に変換
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 描画用インスタンスから描画機能を設定
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読み込み機能を設定
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	// 投影行列を設定
	m_efkRenderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(
			90.0f / 180.0f * 3.14f, (float)w / (float)h, 1.0f, 500.0f));
}

void KdEffekseerManager::Update()
{
	if (m_efkManager == nullptr) { return; }

	std::shared_ptr<CameraBase> camera = m_wpCamera.lock();

	bool vfFlg = false;

	// ターゲットが視錐台外に行ったらレティクルの座標を原点に戻す
	for (auto& obj : m_effectMap)
	{
		// 何も範囲内に入っていなかったら
		if (ViewingFrustumCulling(camera, obj.second->GetPos()))
		{
			vfFlg = true;
		}
	}

	if (vfFlg)
	{
		UpdateEffekseerEffect();

		UpdateEkfCameraMatrix();
	}

}

void KdEffekseerManager::Draw()
{
	if (m_efkManager == nullptr ||
		m_efkRenderer == nullptr) {
		return;
	}

	std::shared_ptr<CameraBase> camera = m_wpCamera.lock();

	bool vfFlg = false;

	// ターゲットが視錐台外に行ったらレティクルの座標を原点に戻す
	for (auto& obj : m_effectMap)
	{
		// 何も範囲内に入っていなかったら
		if (ViewingFrustumCulling(camera, obj.second->GetPos()))
		{
			vfFlg = true;
		}
	}

	if (vfFlg)
	{
		m_efkRenderer->BeginRendering();
		m_efkManager->Draw();
		m_efkRenderer->EndRendering();
	}
}

Effekseer::Handle KdEffekseerManager::Play(
	const std::string& effName, const DirectX::SimpleMath::Vector3& pos, bool loop)
{
	PlayEfkInfo info;

	info.FileName = effName;
	info.Pos = pos;
	info.Size = 1;
	info.Speed = 1;
	info.IsLoop = loop;

	return Play(info);
}

void KdEffekseerManager::StopAllEffect()
{
	if (m_efkManager == nullptr) { return; }

	for (auto&& efk : m_effectMap)
	{
		if (!efk.second->IsLoop()) { continue; }

		efk.second->SetLoop(false);
	}

	m_efkManager->StopAllEffects();
}

void KdEffekseerManager::StopEffect(const Effekseer::Handle& name)
{
	auto foundItr = m_effectMap.find(name);

	if (foundItr == m_effectMap.end()) { return; }

	if (foundItr->second->IsLoop())
	{
		foundItr->second->SetLoop(false);
		return;
	}

	m_efkManager->StopEffect(foundItr->second->GetHandle());
}

void KdEffekseerManager::Release()
{
	Reset();

	m_efkManager.Reset();
	m_efkRenderer.Reset();
}

void KdEffekseerManager::Reset()
{
	StopAllEffect();

	m_effectMap.clear();

	m_isPause = false;
}

void KdEffekseerManager::SetPos(const Effekseer::Handle& handle, const Math::Vector3& pos)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	Effekseer::Vector3D efkPos = GetEfkVec3D(pos);

	m_efkManager->SetLocation(foundItr->second->GetHandle(), efkPos);
}

void KdEffekseerManager::SetRotation(const Effekseer::Handle& handle, const Math::Vector3& axis, const float angle)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	Effekseer::Vector3D efkAxis = GetEfkVec3D(axis);

	m_efkManager->SetRotation(handle, efkAxis, angle);
}

void KdEffekseerManager::SetWorldMatrix(const Effekseer::Handle& handle, const Math::Matrix& mWorld)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	Effekseer::Matrix43 mEfk{};

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mEfk.Value[i][j] = mWorld.m[i][j];
		}
	}

	m_efkManager->SetMatrix(foundItr->second->GetHandle(), mEfk);
}

void KdEffekseerManager::SetScale(const Effekseer::Handle& handle, const Math::Vector3& scale)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	float scaleX = scale.x;
	float scaleY = scale.y;
	float scaleZ = scale.z;

	m_efkManager->SetScale(handle, scaleX, scaleY, scaleZ);
}

void KdEffekseerManager::SetScale(const Effekseer::Handle& handle, const float scale)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	m_efkManager->SetScale(handle, scale, scale, scale);
}

void KdEffekseerManager::SetSpeed(const Effekseer::Handle& handle, const float speed)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	m_efkManager->SetSpeed(handle, speed);
}

void KdEffekseerManager::SetPause(const Effekseer::Handle& handle, const bool isPause)
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return; }

	m_efkManager->SetPaused(handle, isPause);
}

const bool KdEffekseerManager::IsPlaying(const Effekseer::Handle& handle) const
{
	auto foundItr = m_effectMap.find(handle);

	if (foundItr == m_effectMap.end()) { return false; }

	return foundItr->second->IsPlaying();
}

Effekseer::Handle KdEffekseerManager::Play(const PlayEfkInfo& info)
{
	// 渡された座標をEffekseerの座標に置き換え
	Effekseer::Vector3D efkPos = GetEfkVec3D(info.Pos);

	Effekseer::Handle handle = 0;

	std::string loadFileName = EffekseerPath + info.FileName;

	// エフェクト新規生成
	auto effect = Effekseer::Effect::Create(m_efkManager,
		(const EFK_CHAR*)sjis_to_wide(loadFileName).c_str(), info.Size);

	if (effect == nullptr)
	{
#ifdef _DEBUG
		assert(0 && "Effekseerのエフェクト作成失敗");
#endif
		return NULL;
	}

	std::shared_ptr<KdEffekseerObject> spEfkObject = std::make_shared<KdEffekseerObject>();

	handle = m_efkManager->Play(effect, efkPos);

	m_efkManager->SetSpeed(handle, info.Speed);

	spEfkObject->SetParentManager(m_efkManager);
	spEfkObject->SetEffect(effect);
	spEfkObject->SetHandle(handle);
	spEfkObject->SetPlayEfkInfo(info);

	m_effectMap[handle] = spEfkObject;

	return handle;
}

Effekseer::Handle KdEffekseerManager::Play(
	const std::shared_ptr<KdEffekseerObject> spObject)
{
	return Play(spObject->GetPlayEfkInfo());
}

void KdEffekseerManager::UpdateEffekseerEffect()
{
	if (m_isPause) { return; }

	m_efkManager->Update();

	auto efkItr = m_effectMap.begin();

	m_efkManager->BeginUpdate();

	while (efkItr != m_effectMap.end())
	{
		bool isLoop = efkItr->second->IsLoop();

		if (!isLoop)
		{
			++efkItr;
			continue;
		}

		auto handle = efkItr->second->GetHandle();

		if (m_efkManager->GetInstanceCount(handle) == 0)
		{
			Play(efkItr->second);
		}

		++efkItr;
	}

	m_efkManager->EndUpdate();
}

void KdEffekseerManager::UpdateEkfCameraMatrix()
{
	std::shared_ptr<CameraBase> spCamera = nullptr;
	spCamera = m_wpCamera.lock();
	if (!spCamera) return;

	//auto mat = spCamera->GetCamera()->GetCameraMatrix().Invert();

	Math::Matrix mView = spCamera->GetCamera()->GetCameraMatrix().Invert();
	Math::Matrix mProj = spCamera->GetCamera()->GetProjMatrix();
	Effekseer::Matrix44 mEfkView;
	Effekseer::Matrix44 mEfkProj;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mEfkView.Values[i][j] = mView.m[i][j];
			mEfkProj.Values[i][j] = mProj.m[i][j];
		}
	}

	m_efkRenderer->SetProjectionMatrix(mEfkProj);
	m_efkRenderer->SetCameraMatrix(mEfkView);
}

void KdEffekseerManager::SetCamera(const std::shared_ptr<CameraBase>& camera)
{
	m_wpCamera = camera;
}

bool KdEffekseerManager::ViewingFrustumCulling(const std::shared_ptr<CameraBase>& _camera, const Math::Vector3& _pos)
{
	// 視錐台内に対象が入っている場合自動でターゲットを切り替えてレティクルを追尾
	// 当たられるオブジェクトの設定
	DirectX::BoundingBox aabb;
	aabb.Center = _pos;
	aabb.Extents = Math::Vector3(2, 2, 2);

	// aabbをマトリックスを元に変形
	DirectX::BoundingOrientedBox obb;
	DirectX::BoundingOrientedBox::CreateFromBoundingBox(obb, aabb);
	obb.Transform(obb, _camera->GetMatrix());

	// 視錐台
	DirectX::BoundingFrustum vf;
	DirectX::BoundingFrustum::CreateFromMatrix(vf, _camera->GetCamera()->GetProjMatrix());
	Math::Matrix mCam = _camera->GetCamera()->GetCameraMatrix();
	vf.Origin = mCam.Translation();
	vf.Orientation = Math::Quaternion::CreateFromRotationMatrix(mCam);

	// 範囲内に入っていたら
	if (vf.Intersects(obb))
	{
		return true;
	}
	return false;
}

bool KdEffekseerObject::IsPlaying()
{
	if (m_parentManager == nullptr) { return false; }

	// ハンドルが0( 未再生 or 再生終了 )でない場合はTrue, 
	// そうでなければFalse
	return m_parentManager->GetInstanceCount(m_handle) != 0;
}

void KdEffekseerObject::SetPos(const Math::Vector3& pos)
{
	m_info.Pos = pos;

	KdEffekseerManager::GetInstance().SetPos(m_info.Handle, pos);
}

void KdEffekseerObject::SetScale(const float scale)
{
	m_info.Size = scale;

	KdEffekseerManager::GetInstance().SetScale(m_info.Handle, scale);
}

void KdEffekseerObject::SetSpeed(const float speed)
{
	m_info.Speed = speed;

	KdEffekseerManager::GetInstance().SetSpeed(m_info.Handle, speed);
}

void KdEffekseerObject::SetWorldMatrix(const Math::Matrix& mWorld)
{
	KdEffekseerManager::GetInstance().SetWorldMatrix(m_info.Handle, mWorld);
}