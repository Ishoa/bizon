#include "..\Moteur\Interface.h"

#pragma once

class CInterfaceParticule : public CInterface
{
public:


	bool m_bshoot;
	bool m_bControles;

	// sprite declarations
 	
	CLPTexture spriteViseur;

	CInterfaceParticule(CScene* pScene);
	~CInterfaceParticule();

	virtual bool Creation();
	void afficher();

	void draw_Guns();
	void AfficherNbParticules();

	void Release();

};