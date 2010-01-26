#include "..\..\Valkyrie\Moteur\Interface.h"
#include "..\Objects\Planete.h"
#include "..\..\Valkyrie\Moteur\Texture.h"
#pragma once

class CInterfaceSystemSolaire : public CInterface
{
public:


	// sprite declarations

	CLPTexture DisplayTexture;    // the pointer to the texture
	CLPTexture spriteInfo; 	

	bool m_bInfoHide;
	bool m_bInfo_Up;
	bool m_bInfo_Exit;
	bool m_bControles;

	CInterfaceSystemSolaire(CScene* pScene);
	~CInterfaceSystemSolaire();

	bool Creation();
	void afficher(CPlanete* p);

	void draw_display(CPlanete* p);
	void draw_info(CPlanete* p);
	void AfficherControles();
	void AfficherMsgControles();

	void Release();

};