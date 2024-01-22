#include "TitleCamera.h"

void TitleCamera::Init()
{
	// íçéãì_
	m_LocalPos = Math::Matrix::CreateTranslation(0, 1, -12.0f);

	CameraBase::Init();
}

void TitleCamera::Update()
{
	m_mWorld.Translation(m_LocalPos.Translation());

	CameraBase::Update();
}
