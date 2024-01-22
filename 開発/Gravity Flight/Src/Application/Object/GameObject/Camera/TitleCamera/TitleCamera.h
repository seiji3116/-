#pragma once
#include "../CameraBase.h"

class TitleCamera :public CameraBase
{
public:
	TitleCamera() { Init(); }
	virtual ~TitleCamera()	override {}

	void Init()					override;
	void Update()				override;

private:
};