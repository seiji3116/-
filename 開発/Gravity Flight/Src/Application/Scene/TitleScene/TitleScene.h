#pragma once
#include "../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public:

	TitleScene() { Init(); }
	~TitleScene() {}

private:
	//オーバーライドした場合親の処理は行われなくなる
	void Event()override;
	void Init()override;


};