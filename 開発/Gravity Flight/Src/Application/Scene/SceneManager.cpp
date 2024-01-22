#include "SceneManager.h"
#include "BaseScene/BaseScene.h"
#include"GameScene//GameScene.h"
#include"TitleScene/TitleScene.h"

void SceneManager::PreUpdate()
{
	//シーン切り替え
	if (m_currentSceneType != m_nextSceneType && m_blackBackAlpha >= 1)
	{
		ChangeScene(m_nextSceneType);
	}

	//現在のシーンの処理
	m_currentScene->PreUpdate();
}

void SceneManager::Update()
{
	m_currentScene->Update();

	if (m_blackBackFlg)
	{
		m_blackBackAlpha += 0.01f;
		if (m_blackBackAlpha >= 1)
		{
			m_blackBackAlpha = 1;
		}
	}
}

void SceneManager::PostUpdate()
{
	m_currentScene->PostUpdate();
}

void SceneManager::PreDraw()
{
	m_currentScene->PreDraw();
}

void SceneManager::Draw()
{
	m_currentScene->Draw();

}

void SceneManager::DrawSprite()
{
	m_currentScene->DrawSprite();
	
	Math::Rectangle rc = {};
	Math::Color color = {};
	Math::Matrix mat = {};

	// タイトル
	color = Math::Color(1, 1, 1, m_blackBackAlpha);
	rc = Math::Rectangle(0, 0, 1280, 720);
	mat = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, 0));
	KdShaderManager::Instance().m_spriteShader.SetMatrix(mat);
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_blackBackTexture, 0, 0, 1280, 720, &rc, &color);
}

void SceneManager::DrawDebug()
{
	m_currentScene->DrawDebug();
}

const std::list<std::shared_ptr<KdGameObject>> SceneManager::GetObjList()
{
	return m_currentScene->GetObjList();
}

void SceneManager::AddObject(const std::shared_ptr<KdGameObject> _obj)
{
	m_currentScene->AddObject(_obj);
}

void SceneManager::ChangeScene(SceneType _sceneType)
{
	// 現在のシーンを削除
	m_currentScene.reset();
	m_currentSceneType = m_nextSceneType;

	m_blackBackAlpha = 0;
	m_blackBackFlg = false;

	KdEffekseerManager::GetInstance().StopAllEffect();
	KdAudioManager::Instance().StopAllSound();

	//次のシーンを作成し、現在のシーンにする
	switch (_sceneType)
	{
	case SceneType::Title:
		m_currentScene = std::make_shared<TitleScene>();
		KdAudioManager::Instance().Play("Asset/Sounds/Game/GameBGM.wav", true, 0.5f);
		break;
	case SceneType::Game:
		m_currentScene = std::make_shared<GameScene>();
		KdAudioManager::Instance().Play("Asset/Sounds/Title/TitleBGM.wav", true, 0.f);
		break;
	}
	m_currentSceneType = _sceneType;
}
