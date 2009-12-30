#ifndef _FUSEEARTIFICE_H_
#define _FUSEEARTIFICE_H_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Particlesystem.h"

class CFuseeArtifice
{
public:
	enum EEtatFuseeArtifice
	{	
		e_Lancement,
		e_Fusee,
		e_Wait,
		e_Explode,
		e_Finish,
	};

protected:

	EEtatFuseeArtifice e_EtatFuseeArtifice;

	CParticleSystem* m_pParticleSystemFusee;
	CParticleSystem* m_pParticleSystemExplode;

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	float m_fTimeToExplode;
	float m_fTimeAfterExplode;
	float m_fCurrentTime;
	int m_nSigne;

	D3DXVECTOR3 m_vPositionDepart;
	float m_fDistanceExplode;
	D3DXVECTOR3 m_vDir;
	float m_fVitessefusee;
	float m_fTimeAvantLancement;
	bool m_bBoucle;

public:
	CFuseeArtifice(LPDIRECT3DDEVICE9 pd3dDevice);
	~CFuseeArtifice();

	bool CreateAndInit(char **g_pTextNameFusee , int nTailleFusee,char **g_pTextNameExplode , int nTailleExplode);
	void DestructionObjet();
	void Rendu3D();
	void Animation(float fDeltaTemps);


	void SetVitesseFusee(float fVitessefusee){m_fVitessefusee = fVitessefusee;}
	void SetDistanceExplode(float fDistanceExplode){m_fDistanceExplode = fDistanceExplode;}
	void SetTimeAfterExplode(float fTimeAfterExplode){m_fTimeAfterExplode = fTimeAfterExplode;}
	void SetDirrectionFusee(D3DXVECTOR3 vDir){m_vDir = vDir;}
	void SetPositionDepart(D3DXVECTOR3 vPositionDepart){m_vPositionDepart = vPositionDepart;}
	void SetTimeAvantLancement(float fTimeAvantLancement){m_fTimeAvantLancement = fTimeAvantLancement;}
	void SetBoucle(bool bBoucle){m_bBoucle = bBoucle;};
};

#endif