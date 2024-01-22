#include "TPSCamera.h"

void TPSCamera::Init()
{
	// 注視点
	m_LocalPos = Math::Matrix::CreateTranslation(0, 2.5f, -10.0f);

	//画面中央
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	//SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void TPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			//targetMat = Math::Matrix::CreateTranslation(spTarget->GetPos());
			targetMat = spTarget->GetMatrix();
		}
	}

	//カメラの回転
	m_Rotation = GetRotationMatrix();

	// カメラの最新データ
	m_ghostMat[0] = m_LocalPos * m_Rotation * targetMat;

	// 最後から1つ前のデータをコピー
	for (int i = GHOST_MAX - 1; i > 0; i--)
	{
		m_ghostMat[i] = m_ghostMat[i - 1];
	}

	m_mWorld = m_ghostMat[9];

	CameraBase::Update();
}
