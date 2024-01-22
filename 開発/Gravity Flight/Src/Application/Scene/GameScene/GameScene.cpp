#include "GameScene.h"
#include "../../Object/GameObject/Player/Player.h"
#include "../../Object/GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../Object/GameObject/Sky/Sky.h"
#include "../../Object/GameObject/Rock/Rock.h"
#include "../../Object/GameObject/Effect/BackGroundEffect/BackGroundEffect.h"
#include "../../Object/GameObject/Enemy/Enemy.h"
#include "../../Object/GameObject/MotherEnemy/MotherEnemy.h"
#include "../../Object/GameObject/BasePlanet/BasePlanet.h"
#include "../../Object/GameObject/Landmark/Landmark.h"
#include "../../Object/GameObject/Laser/Laser.h"
#include "../../Object/GameObject/UI/GameUI/Reticle/Reticle.h"
#include "../../Object/GameObject/UI/GameUI/ConcentratioLine/ConcentratioLine.h"
#include "../../Object/GameObject/ResultUI/ResultUI.h"

#include "../SceneManager.h"

void GameScene::Event()
{
	if (KdInputManager::Instance().IsPress("scene") && m_sceenChengeFlg)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
		KdAudioManager::Instance().Play("Asset/Sounds/Utility/Click.wav", false, 0.3f);
	}

	if (!m_sceenChengeFlg)
	{
		if (m_motherEnemy->GetGameClearFlg())
		{
			m_resultUI->GameClearFlgOn();
			m_player->SetMoveSpd(0);
			m_player->SetActionFlg(false);
			m_player->SetGameClearFlg(true);
			m_sceenChengeFlg = true;
		}
		if (m_player->GetGameOverFlg())
		{
			m_resultUI->GameOverFlgOn();
			m_sceenChengeFlg = true;
		}
	}
}

void GameScene::Init()
{
	m_resultUI = std::make_shared<ResultUI>();
	AddObject(m_resultUI);

	m_player = std::make_shared<Player>();
	AddObject(m_player);

	std::shared_ptr<TPSCamera> camera = std::make_shared<TPSCamera>();
	camera->SetTarget(m_player);
	m_player->SetCamera(camera);
	KdEffekseerManager::GetInstance().SetCamera(camera);
	AddObject(camera);

	std::shared_ptr<Base> base;
	base = std::make_shared<Base>();
	AddObject(base);

	m_motherEnemy = std::make_shared<MotherEnemy>();
	m_motherEnemy->SetPlayer(m_player);
	AddObject(m_motherEnemy);

	std::shared_ptr<Enemy> enemy;
	//enemy = std::make_shared<Enemy>();
	//enemy->SetPlayer(player);
	//enemy->SetState("Normal");
	//player->PushBackTargetList(enemy);
	//AddObject(enemy);

	// Šâ‰Šú‰»
	std::shared_ptr<Rock> rock;
	std::list<std::shared_ptr<Enemy>> laserEnemyList;

	for (int i = 0; i < 3; i++)
	{
		enemy = std::make_shared<Enemy>();
		enemy->SetState("Laser");
		enemy->SetPlayer(m_player);
		enemy->SetPos(m_motherEnemy->GetNodePos("LaserPos", i));
		enemy->SetShieldNum(i + 1);
		enemy->SetCamera(camera);
		m_player->PushBackTargetList(enemy);
		laserEnemyList.push_back(enemy);
		m_motherEnemy->PushBackLaserEnemyList(enemy);
		AddObject(enemy);
	}

	float radius = 120;
	int detail = 110;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < detail + 1; ++j)
		{
			Math::Vector3 pos;
			pos = base->GetPos();
			pos.x += cos((float)j * (360 / detail) * KdToRadians) * radius;
			pos.y += cos((float)j * (360 / detail) * KdToRadians) * radius * i;
			pos.z += sin((float)j * (360 / detail) * KdToRadians) * radius;

			rock = std::make_shared<Rock>();
			rock->SetOwnerPos(base->GetPos());
			rock->SetPos(pos);
			rock->SetRevolutionPos(base->GetPos() - pos);
			rock->SetPlayer(m_player);
			for (auto& lenemy : laserEnemyList)
			{
				rock->PushBuckLaserEnemyList(lenemy);
			}
			AddObject(rock);
		}
		detail += 50;
		radius += 5;
	}

	std::shared_ptr<Sky> sky;
	sky = std::make_shared<Sky>();
	sky->SetPlayer(m_player);
	AddObject(sky);

	std::shared_ptr<Landmark> landmark;
	landmark = std::make_shared<Landmark>();
	for (int i = 1; i <= 5; i++)
	{
		std::string name = "Landmark" + std::to_string(i);
		landmark->PushBackPlanetMap(name, sky->GetNodePos(name));
	}
	AddObject(landmark);


	for (int i = 1; i <= 20; i++)
	{
		std::shared_ptr<BackGroundEffect> effect;
		effect = std::make_shared<BackGroundEffect>();
		std::string num = "Effect" + std::to_string(i);
		effect->SetPos(sky->GetNodePos(num));
		effect->SetCamera(camera);
		AddObject(effect);
	}

	// ƒŒƒeƒBƒNƒ‹’Ç‰Á
	std::shared_ptr<Reticle> reticle;
	reticle = std::make_shared<Reticle>();
	m_player->SetReticle(reticle);
	AddObject(reticle);

	std::shared_ptr<ConcentratioLine> concentratioLine;
	concentratioLine = std::make_shared<ConcentratioLine>();
	m_player->SetConcentratioLine(concentratioLine);
	AddObject(concentratioLine);
}