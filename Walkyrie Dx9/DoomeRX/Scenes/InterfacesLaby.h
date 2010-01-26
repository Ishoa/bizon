#include "..\..\Valkyrie\Moteur\Interface.h"

#pragma once

class CInterfaceLaby : public CInterface
{
public:

	float enemyX;
	float enemyY;
	int health;       // the player's current hitpoints
	int maxhealth;    // the player's max hitpoints
	int ammo;         // the player's current ammo

	bool m_bshoot;
	bool m_bControles;

	// sprite declarations

	CLPTexture DisplayTexture;    // the pointer to the texture 	
	CLPTexture spriteGuns; 	
	CLPTexture spriteViseur;
	CLPTexture spriteImpact;

	CInterfaceLaby(CScene* pScene);
	~CInterfaceLaby();

	virtual bool Creation();
	void afficher();

	void draw_display();
	void draw_Guns();
	void draw_Impact();

	void AfficherControles();
	void AfficherMsgControles();

	void Release();

};