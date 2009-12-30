#include "InterfacesLaby.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

CInterfaceLaby::CInterfaceLaby(CScene* pScene) : CInterface(pScene)
{

	enemyX = 60.0f;
	enemyY = 60.0f;   
	health = 300;
	maxhealth = 1000;
	ammo = 200;

	m_bshoot = false;

	m_dxfont = NULL;

	m_bControles = false;
}

CInterfaceLaby::~CInterfaceLaby()
{
}

bool CInterfaceLaby::Creation()
{
	m_pD3DSpt = m_pScene->GetMoteur()->GetMoteur3D()->GetSprites();
	m_LargeurFenetre=m_pScene->GetMoteur()->m_LargeurFenetre;	
	m_HauteurFenetre=m_pScene->GetMoteur()->m_HauteurFenetre;


	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &m_dxfont);


	if(!DisplayTexture.chargerTextureFromFileEx(m_pD3DDevice, "..\\Sprites\\DisplaySprites.png", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), NULL, NULL ))
		return false; 

	
	//pistolet.ppm
	//HL2weaponViewModelScreenshot.png
	//arjn2.png
	//viseur.png

	
	if(!spriteViseur.chargerTextureFromFileEx(m_pD3DDevice, "..\\Sprites\\viseur3.png", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL))
		return false; 

	if(!spriteGuns.chargerTextureFromFileEx(m_pD3DDevice, "..\\Sprites\\Weapon.png", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL))
		return false; 
	

	if (!spriteImpact.chargerTextureFromFileEx(m_pD3DDevice,    // the device pointer
		"..\\Sprites\\Doom-Projectiles.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(0, 255, 255),    // the magenta color key
		NULL,    // no image info struct
		NULL))    // not using 256 colors
		return false;    // load to sprite


	return true;
}




void CInterfaceLaby::afficher()
{

	m_pD3DSpt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing with transparency

	
	draw_display();
	draw_Guns();
	if(m_bshoot)
		draw_Impact();

	if(m_bControles)
		AfficherControles();
	else
		AfficherMsgControles();


	AfficherFPS();

	m_pD3DSpt->End();    // end sprite drawing

	return;
}



void CInterfaceLaby::AfficherControles()
{
	// create a RECT to contain the text
	static RECT textbox;
	SetRect(&textbox, 20,200, 510, 460); 

	char pDescControles[]  = "(touche N pour Desactiver l'aide)\nZ\/Q\/S\/D : Touches Directionnelles\nMouse Gauche : Tir\nR : Reload\nG : Grenade\nF1\/F2\/F3 : Type filtrage texture\nF5\/F6 : Affichage Solid ou Fil de Fer\nF9\/F10 : Activation\/Desactivation affichage boites englobantes";
	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		pDescControles,
		sizeof(pDescControles),
		&textbox,
		DT_LEFT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}

void CInterfaceLaby::AfficherMsgControles()
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



void CInterfaceLaby::draw_Guns()
{
	RECT Part;

	m_pScene->GetMoteur()->GetMoteur3D()->GetDevice3D()->SetRenderState(D3DRS_WRAP0,0);

	// DRAW THE RADAR
	// display the backdrop
	SetRect(&Part,0, 0, 150,150);
	
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f), position(m_LargeurFenetre/2, m_HauteurFenetre/2, 0.0f);
	m_pD3DSpt->Draw(spriteGuns.getDXTexture(), NULL, &center, &position, D3DCOLOR_ARGB(255,255, 255, 255));

	SetRect(&Part,0, 0, 50,50);

	D3DXVECTOR3 center2(15.0f, 15.0f, 0.0f);
	D3DXVECTOR3 position2(m_LargeurFenetre/2, m_HauteurFenetre/2, 0.0f);
	m_pD3DSpt->Draw(spriteViseur.getDXTexture(), NULL, &center2, &position2, D3DCOLOR_ARGB(180,255, 255, 255));


	//DrawTextureSprites(spriteGuns, Part, m_LargeurFenetre/2+110, m_HauteurFenetre-120, 255);


}


void CInterfaceLaby::draw_Impact()
{

	static int frame = 100;
	
	RECT Part;
	D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);

	if(frame>75)
	{
 		center.x = 5;
 		center.y = 12;
		SetRect(&Part,0, 10, 10,25);
	}
	else if(frame>50)
	{		
 		center.x = 7;
 		center.y = 8;
		SetRect(&Part,10, 8, 23,25);
	}
	else if(frame>25)
	{
 		center.x = 7;
 		center.y = 11;
		SetRect(&Part,22, 3, 36,25);
	}
	else if(frame>0)
	{
 		center.x = 11;
 		center.y = 12;
		SetRect(&Part,38, 0, 60,25);
	}
	else 
	{
		frame = 101;
		m_bshoot = false;
	}
	frame--;

	
	D3DXVECTOR3 position(m_LargeurFenetre/2, m_HauteurFenetre/2, 0.0f);
	m_pD3DSpt->Draw(spriteImpact.getDXTexture(), &Part, &center, &position, D3DCOLOR_ARGB(255,255, 255, 0));

}



// this draws the display
void CInterfaceLaby::draw_display()
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
		D3DCOLOR_ARGB(255, 0, 0, 255));

	return;
}


void CInterfaceLaby::Release()
{
	SAFE_RELEASE(m_dxfont);

	DisplayTexture.Release();	
	spriteGuns.Release(); 	
	spriteViseur.Release();
	spriteImpact.Release();

	m_dxfont = NULL;

}