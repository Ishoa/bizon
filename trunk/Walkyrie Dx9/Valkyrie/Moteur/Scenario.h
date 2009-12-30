#ifndef _CSCENARIO_H_
#define _CSCENARIO_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Moteur.h"
#include "Scene.h"

#define MAX_SCENE 10

class CScene;

class CScenario
{

public:
	enum EEtatRendu
	{
		e_RenduScene3D,
		e_RenduCinematic,
	};

protected:

	CMoteur* m_pMoteur;
	CScene* m_pCurrentScene;

	EEtatRendu m_eEtatRendu;

public:

	CScenario();
	~CScenario();

	virtual bool Creation();
	virtual bool Init();
	virtual bool Reset();

	virtual void Rendu();
	virtual void RenduScene3D();
	virtual void RenduCinematic();

	virtual void CreationObjet();
	void SetRatioFenetre(float fRatio);

	virtual void DestructionFinal();
	virtual void DestructionObjet();

};

#endif