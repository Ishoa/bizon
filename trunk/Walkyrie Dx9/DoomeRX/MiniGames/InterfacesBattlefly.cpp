#include "InterfacesBattlefly.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

CInterfaceBattleFly::CInterfaceBattleFly(CScene* pScene) : CInterface(pScene)
{

	m_bInfoHide = true;
	m_bInfo_Up = false;
	m_bInfo_Exit = false;
	m_bControles =  false;

	m_dxfont = NULL;

	enemyX = 60.0f;
	enemyY = 60.0f;   
	health = 300;
	maxhealth = 1000;
	ammo = 10394;
}

CInterfaceBattleFly::~CInterfaceBattleFly()
{

}



void CInterfaceBattleFly::Release()
{
	DisplayTexture.Release();
	spriteInfo.Release();

	SAFE_RELEASE(m_dxfont);

	m_dxfont = NULL;

}

bool CInterfaceBattleFly::Creation()
{

	m_pD3DSpt = m_pScene->GetMoteur()->GetMoteur3D()->GetSprites();
	m_LargeurFenetre=m_pScene->GetMoteur()->m_LargeurFenetre;	
	m_HauteurFenetre=m_pScene->GetMoteur()->m_HauteurFenetre;

	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
               OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
               "Arial", &m_dxfont);


	if(!DisplayTexture.chargerTextureFromFileEx(m_pD3DDevice,"..\\Sprites\\DisplaySprites.png", D3DX_DEFAULT, D3DX_DEFAULT,
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
                                NULL))    // not using 256 colors)
				return false;    // load to sprite
	return true;
}




void CInterfaceBattleFly::afficher()
{

   	m_pD3DSpt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing with transparency

    draw_display();
	draw_info();

	if(m_bControles)
		AfficherControles();
	else
		AfficherMsgControles();


	AfficherFPS();

    m_pD3DSpt->End();    // end sprite drawing

	return;
}

void CInterfaceBattleFly::AfficherControles()
{
	// create a RECT to contain the text
	static RECT textbox;
	SetRect(&textbox, 20,200, 500, 460); 

	char pDescControles[]  = "(touche N pour Desactiver l'aide)\nSPACE : Description Terrain \nF1 : Camera 360°\nF2 : Camera Premiere Personne\nF3 : Camera Avion\F4 : Camera Vol Libre\nF5\/F6\/F7\/F8 : Type de filtrage des Textures\nF9\/F10 : Plein\/Rendu Fil de Fer ";
	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		pDescControles,
		sizeof(pDescControles),
		&textbox,
		DT_LEFT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}

void CInterfaceBattleFly::AfficherMsgControles()
{
	// create a RECT to contain the text
	static RECT textbox;
	SetRect(&textbox, 20,200, 250, 240); 

	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		"(touche H pour Activer l'aide)" ,
		30,
		&textbox,
		DT_LEFT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}


// this draws the display
void CInterfaceBattleFly::draw_display()
{
    RECT Part;

    // DRAW THE RADAR
    // display the backdrop
    SetRect(&Part, 2, 14, 169, 181);
    DrawTextureSprites(DisplayTexture, Part, 10, 10, 127);

    // if the enemy is within 84 units of the player, display the enemy
    if(sqrt((92 - enemyX) * (92 - enemyX) + (92 - enemyY) * (92 - enemyY)) < 84)
    {
        SetRect(&Part, 341, 14, 344, 17);
        DrawTextureSprites(DisplayTexture, Part, enemyX, enemyY, 255);
    }

    // display the border
    SetRect(&Part, 171, 13, 340, 182);
    DrawTextureSprites(DisplayTexture, Part, 9, 9, 255);

    // DRAW THE HEALTHBAR
    // display the bar
    SetRect(&Part, 1, 1, 505, 12);
    DrawTextureSprites(DisplayTexture, Part, 11.0f, m_HauteurFenetre-24.0f, 255);

    // display the health "juice"
    SetRect(&Part, 506, 1, 507, 12);
    for(int index = 0; index < (health * 490 / maxhealth); index++)
        DrawTextureSprites(DisplayTexture, Part, index + 18.0f, m_HauteurFenetre-24.0f, 255);

    // DRAW THE AMMO INDICATOR
    // display the backdrop
    SetRect(&Part, 351, 14, 456, 40);
    DrawTextureSprites(DisplayTexture, Part, m_LargeurFenetre-110.0f, m_HauteurFenetre-31.0f, 127);


	// display the border
    SetRect(&Part, 351, 45, 457, 72);
    DrawTextureSprites(DisplayTexture, Part, m_LargeurFenetre-110.0f, m_HauteurFenetre-31.0f, 255);

    // display the font
	SetRect(&Part, m_LargeurFenetre-105, m_HauteurFenetre-27, m_LargeurFenetre-10, m_HauteurFenetre-10);
    //SetRect(&Part, 535, 453, 630, 470);
    static char strAmmoText[10];
    _itoa_s(ammo, strAmmoText, 10);
    m_dxfont->DrawTextA(NULL,
                      (LPCSTR)&strAmmoText,
                      strlen((LPCSTR) &strAmmoText),
                      &Part,
                      DT_RIGHT,
                      D3DCOLOR_ARGB(255, 120, 120, 255));

    return;
}

void CInterfaceBattleFly::draw_info()
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

		if(m_bInfo_Up && frame < 21.0f) frame+=0.2f;     // if we aren't on the last frame, go to the next frame
		if(m_bInfo_Exit && frame > 0.0f)	frame-=0.2f;
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

		char pDesc[] ="TERRAIN 01";

		if(((int)frame)==21)
			// draw the Hello World text
			m_dxfont->DrawTextA(m_pD3DSpt,
			pDesc,
			10,
			&textbox,
			DT_LEFT,
			D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	return;
}
