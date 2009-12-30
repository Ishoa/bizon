#include "..\Moteur\Interface.h"
#include "Texture.h"

#pragma once

class CInterfaceBattleFly : public CInterface
{
	public:

		bool m_bInfoHide;
		bool m_bInfo_Up;
		bool m_bInfo_Exit;
		bool m_bControles;

		float enemyX;
		float enemyY;
		int health;       // the player's current hitpoints
		int maxhealth;    // the player's max hitpoints
		int ammo;         // the player's current ammo

				
		// sprite declarations

		CLPTexture DisplayTexture;    // the pointer to the texture
		CLPTexture spriteInfo; 	

		CInterfaceBattleFly(CScene* pScene);
		~CInterfaceBattleFly();

		bool Creation();
		void afficher();

		void draw_display();
		void draw_info();
		void AfficherControles();
		void AfficherMsgControles();
		void Release();
	
};