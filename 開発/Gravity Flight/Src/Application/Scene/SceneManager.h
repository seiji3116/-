#pragma once
class BaseScene;

class SceneManager
{
public:
	//シーン情報
	enum class SceneType
	{
		Title,
		Game,
	};

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	//マネージャの初期化関数
	void Init() 
	{ 
		ChangeScene(SceneType::Title); 
		m_blackBackTexture.Load("Asset/Textures/UI/Utility/BlackBack.png");
	}

	//void Release();

	//この関数ではシーンは変わらない
	//あくまで予約
	void SetNextScene(SceneType _nextScene) 
	{ 
		m_blackBackFlg = true;
		m_nextSceneType = _nextScene; 
	}

	const std::list<std::shared_ptr<KdGameObject>> GetObjList();

	void AddObject(const std::shared_ptr<KdGameObject> _obj);
private:

	void ChangeScene(SceneType _sceneType);

	//現在のシーンを管理しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;
	//BaseScene *m_currentScene = nullptr;

	//現在のシーンを管理しているデータ（変数）
	SceneType m_currentSceneType = SceneType::Title;
	//次のシーンを管理しているデータ（変数）
	SceneType m_nextSceneType = m_currentSceneType;

	// 黒背景
	KdTexture m_blackBackTexture;
	float m_blackBackAlpha = 0;
	bool m_blackBackFlg = false;

	//シングルトンパターン
	//生成するインスタンスの数を１つに制限する
	//便利過ぎる
	//多用しない！
private:
	SceneManager() { Init(); }
	~SceneManager(){}

public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};