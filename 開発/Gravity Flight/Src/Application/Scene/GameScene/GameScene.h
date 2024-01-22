#pragma once
#include "../BaseScene/BaseScene.h"

class Player;
class MotherEnemy;
class ResultUI;

class GameScene : public BaseScene
{
public:

	GameScene() { Init(); }
	~GameScene() {}

private:
	//�I�[�o�[���C�h�����ꍇ�e�̏����͍s���Ȃ��Ȃ�
	void Event()override;
	void Init()override;

	bool m_sceenChengeFlg = false;;

	std::shared_ptr<MotherEnemy> m_motherEnemy;
	std::shared_ptr<Player> m_player;
	std::shared_ptr<ResultUI> m_resultUI;
};