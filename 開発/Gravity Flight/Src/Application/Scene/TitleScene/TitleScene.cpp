#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Object/TitleObject/Player/TitlePlayer.h"
#include "../../Object/TitleObject/Sky/TitleSky.h"
#include "../../Object/TitleObject/UI/TitleUI.h"
#include "../../Object/TitleObject/Rock/TitleRock.h"
#include "../../Object/GameObject/Camera/TitleCamera/TitleCamera.h"

void TitleScene::Event()
{
	if (KdInputManager::Instance().IsPress("scene"))
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);
		KdAudioManager::Instance().Play("Asset/Sounds/Utility/Click.wav", false, 0.3f);
	}
}

void TitleScene::Init()
{
	std::shared_ptr<TitlePlayer> player;
	player = std::make_shared<TitlePlayer>();
	player->SetPos(Math::Vector3(0, 0, 0));
	AddObject(player);

	// デバッグラインの応用 半径と個数を入れると、円形に座標を入れる
	std::shared_ptr<TitleRock> rock;
	std::vector<Math::Vector3> vPos;
	float radius = 6;
	int detail = 3;
	for (int j = 0; j < detail + 1; ++j)
	{
		Math::Vector3 pos;
		pos = player->GetPos();
		pos.x += cos((float)j * (360 / detail) * KdToRadians) * radius;
		pos.y += cos((float)j * (360 / detail) * KdToRadians) * radius;
		pos.z += sin((float)j * (360 / detail) * KdToRadians) * radius;
		vPos.push_back(pos);
	}

	// 保存した座標から回転軸を出す
	Math::Vector3 axis = Math::Vector3::Zero;
	axis = (vPos[0] - vPos[1]).Cross(vPos[2] - vPos[1]);

	for (int i = 0; i < vPos.size(); i++)
	{
		rock = std::make_shared<TitleRock>();
		rock->SetPos(vPos[i]);
		rock->SetLocalTrans(player->GetPos() - vPos[i]);
		rock->SetPlayer(player);
		rock->SetAxis(axis);
		AddObject(rock);
	}

	std::shared_ptr<TitleSky> sky;
	sky = std::make_shared<TitleSky>();
	AddObject(sky);

	std::shared_ptr<TitleUI> ui;
	ui = std::make_shared<TitleUI>();
	AddObject(ui);

	std::shared_ptr<TitleCamera> camera = std::make_shared<TitleCamera>();
	KdEffekseerManager::GetInstance().SetCamera(camera);
	AddObject(camera);
}
