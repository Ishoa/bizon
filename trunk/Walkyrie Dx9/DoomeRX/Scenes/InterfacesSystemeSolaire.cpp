#include "InterfacesSystemeSolaire.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

CInterfaceSystemSolaire::CInterfaceSystemSolaire(CScene* pScene) : CInterface(pScene)
{
	m_bInfoHide = true;
	m_bInfo_Up = false;
	m_bInfo_Exit = false;
	m_bControles =  false;

}

CInterfaceSystemSolaire::~CInterfaceSystemSolaire()
{

}

void CInterfaceSystemSolaire::Release()
{
	CInterface::Release();
	DisplayTexture.Release();
	spriteInfo.Release();
}

bool CInterfaceSystemSolaire::Creation()
{
	m_pD3DSpt = m_pScene->GetMoteur()->GetMoteur3D()->GetSprites();
	m_LargeurFenetre=m_pScene->GetMoteur()->m_LargeurFenetre;	
	m_HauteurFenetre=m_pScene->GetMoteur()->m_HauteurFenetre;

	D3DXCreateFont(m_pD3DDevice, 16, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &m_dxfont);


	if(!DisplayTexture.chargerTextureFromFileEx(m_pD3DDevice, "..\\Sprites\\DisplaySprites.png", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), NULL, NULL))
		return false; 


	if (!spriteInfo.chargerTextureFromFileEx(m_pD3DDevice,    // the device pointer
		"..\\Sprites\\Panel3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL))    // not using 256 colors
		return false;    // load to sprite

	return true;
}




void CInterfaceSystemSolaire::afficher(CPlanete* p)
{

	m_pD3DSpt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing with transparency

	//draw_display();
	if(m_bControles)
		AfficherControles();
	else
		AfficherMsgControles();
	draw_info(p);
	
	AfficherFPS();
	m_pD3DSpt->End();    // end sprite drawing

	return;
}



// this draws the display
void CInterfaceSystemSolaire::draw_display(CPlanete* p)
{
	RECT Part;

	// DRAW THE RADAR
	// display the backdrop
	SetRect(&Part, 2, 14, 169, 181);
	DrawTextureSprites(DisplayTexture, Part, 10, 10, 127);

	// if the enemy is within 84 units of the player, display the enemy
	if(sqrt((92 - 0.0f) * (92 - 0) + (92 - 0) * (92 - 0)) < 84)
	{
		SetRect(&Part, 341, 14, 344, 17);
		DrawTextureSprites(DisplayTexture, Part, 0, 0, 255);
	}


	// display the border
	SetRect(&Part, 171, 13, 340, 182);
	DrawTextureSprites(DisplayTexture, Part, 9, 9, 255);


	return;
}

void CInterfaceSystemSolaire::AfficherControles()
{
	// create a RECT to contain the text
	static RECT textbox;
	SetRect(&textbox, 20,20, 500, 400); 

	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		"(touche N pour Desactiver l'aide)\n Help Controlers :\n1\/2 : Augmenter\/Diminuer dScale\n3\/4 : Augmenter\/Diminuer bScale\n5\/6 : Activer\/desactiver lumière\n7\/8 : Augmenter/Diminuer vitesse de rotation du systeme\nF1 : Filtrage Bilinéaire\nF2 : Filtrage Trilinéaire\nF3 : Filtrage Anisotropic\nF5 : Caméra Soleil\nF6 : Camera Terre\nF7 : Camera Lune\nF8 : Camera Mars\nF9 : Camera Mercure\nF10 : Camera Venus\nF11: Camera Pluton\nSPACE : Decsription de la Planete",
						444,
						&textbox,
						DT_LEFT,
						D3DCOLOR_ARGB(255, 255, 0, 0));
}

void CInterfaceSystemSolaire::AfficherMsgControles()
{
	// create a RECT to contain the text
	static RECT textbox;
	SetRect(&textbox, 20,20, 210, 80); 

	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		"(touche H pour Activer l'aide)" ,
		30,
		&textbox,
		DT_LEFT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}

void CInterfaceSystemSolaire::draw_info(CPlanete* p)
{

	// count from 0 to 22 to determine the current frame
	static float frame = 22.0f;    // start the program on the final frame
	
	
	if(KEY_DOWN(VK_SPACE)) 
	{
		if(m_bInfoHide)
		{
			m_bInfoHide = false;
			m_bInfo_Up = true;
			frame=0.0f;     // when the space key is pressed, start at frame 0
		}
		else
		{
			if(frame>=21)
			{
				m_bInfo_Up = false;
				m_bInfo_Exit =  true;
			}
		}
	}

	if (!m_bInfoHide)
	{

		if(m_bInfo_Up && frame < 21.0f) frame+=0.05f;     // if we aren't on the last frame, go to the next frame
		if(m_bInfo_Exit && frame > 0.0f)	frame-=0.05f;
		if(frame <= 0.0f ) 
			{
				frame = 0.0f;
				m_bInfoHide = true;
				m_bInfo_Exit = false;
				m_bInfo_Up = false;
			}	

		// calculate the x-position
		int xpos = ((int)frame) * 182 + 1;

		// draw the selected frame using the coordinates
		RECT part;
		SetRect(&part, xpos, 0, xpos + 181, 128);
		D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(m_LargeurFenetre-190, 10.0f, 0.0f);    // position at 50, 50 with no depth
		m_pD3DSpt->Draw(spriteInfo.getDXTexture(), &part, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));

		//SCREEN_WIDTH
		//SCREEN_HEIGHT

		// create a RECT to contain the text
		static RECT textbox;
		SetRect(&textbox, m_LargeurFenetre-180,20, m_LargeurFenetre, 200); 

		int nTailleDesc = 0;
		int* pRefTaille = &nTailleDesc;
		char * pDesc = p->getDescription(pRefTaille);

		if(((int)frame)==21)
			// draw the Hello World text
			m_dxfont->DrawTextA(m_pD3DSpt,
			pDesc,
			nTailleDesc-1,
			&textbox,
			DT_LEFT,
			D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	return;
}
