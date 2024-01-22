#pragma once
#include "../CameraBase.h"


class TPSCamera : public CameraBase
{
public:
	TPSCamera() { Init(); }
	virtual ~TPSCamera()	override {}

	void Init()					override;
	void Update()				override;

private:

	POINT m_FixMousePos;

	// çÏã∆ópçsóÒ
	static const int GHOST_MAX = 60;
	Math::Matrix m_ghostMat[GHOST_MAX];
};