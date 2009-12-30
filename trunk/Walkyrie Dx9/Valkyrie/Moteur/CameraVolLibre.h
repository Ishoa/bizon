
// Classe pour la gestion d'une caméra

#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "Camera.h"

class CScene;

class CCameraVolLibre : public CCamera
{
public:

	CCameraVolLibre();
	CCameraVolLibre(CScene* pScene);
	~CCameraVolLibre();

	void Animation(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick, float DeltaTemps);

};
