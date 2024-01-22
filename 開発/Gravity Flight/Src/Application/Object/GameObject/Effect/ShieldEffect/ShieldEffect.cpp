#include "ShieldEffect.h"

void ShieldEffect::Init()
{
	m_firstPlay = false;
}

void ShieldEffect::Update()
{
	if (!m_firstPlay)
	{
		m_firstPlay = true;
		m_efkName = "Shield/" + m_efkName + ".efk";
		if (m_handle == 100000)
		{
			m_handle = KdEffekseerManager::GetInstance().Play(m_efkName, m_mWorld.Translation(), false);
			KdEffekseerManager::GetInstance().SetScale(m_handle, 5.f);
		}
	}
}

void ShieldEffect::StopEffect()
{
	KdEffekseerManager::GetInstance().StopEffect(m_handle);
}
