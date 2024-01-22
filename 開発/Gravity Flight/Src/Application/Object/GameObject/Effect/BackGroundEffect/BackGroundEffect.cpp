#include "BackGroundEffect.h"

void BackGroundEffect::Init()
{
	m_rePlayCnt = Random(50, 180);
	m_firstPlayFlg = false;
}

void BackGroundEffect::Update()
{
	if (KdEffekseerManager::GetInstance().IsPlaying(m_handle))
	{
		KdEffekseerManager::GetInstance().SetScale(m_handle, 5.f);
	}

	if (!KdEffekseerManager::GetInstance().IsPlaying(m_handle) && m_firstPlayFlg)
	{
		m_rePlayCnt--;
		if (m_rePlayCnt <= 0)
		{
			m_rePlayCnt = Random(50, 180);
			m_handle = KdEffekseerManager::GetInstance().Play("ToonHit/ToonHit.efk", m_mWorld.Translation(), false);
		}
	}
	if (!m_firstPlayFlg)
	{
		m_rePlayCnt--;
		if (m_rePlayCnt <= 0)
		{
			m_rePlayCnt = Random(50, 180);
			m_handle = KdEffekseerManager::GetInstance().Play("ToonHit/ToonHit.efk", m_mWorld.Translation(), false);
		}
	}
}
