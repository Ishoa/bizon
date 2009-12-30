#ifndef _FUSEEMISSILEEXPLODE_H
#define _FUSEEMISSILEEXPLODE_H

#include "Particlesystem.h"
#include "Scene.h"

class CFuseeMissileExplode
{
protected:

	enum EEtatFuseeExplode
	{	
		e_Lancement,
		e_Fusee,
		e_Wait,
		e_Explode,
		e_Finish,
	};


	EEtatFuseeExplode e_EtatFuseeExplode;

	CParticleSystem* m_pParticleSystemFusee;

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	D3DXVECTOR3 m_vPositionDepart;
	float m_fVitessefusee;
	float m_fDistanceExplode;
	D3DXVECTOR3 m_vDir;

	bool m_bGauche;

	CScene* m_pScene;

public:

	CFuseeMissileExplode(CScene* pScene);
	CFuseeMissileExplode();

	bool CreateAndInit(char **g_pTextNameFusee , int nTailleFusee,char **g_pTextNameExplode , int nTailleExplode);
	void DestructionObjet();
	void Rendu3D();
	void Animation(float fDeltaTemps, bool bTir);

	void SetPositionDepart(D3DXVECTOR3 vPositionDepart){m_vPositionDepart = vPositionDepart;}
	void SetVitesseFusee(float fVitessefusee){m_fVitessefusee = fVitessefusee;}

};

#endif