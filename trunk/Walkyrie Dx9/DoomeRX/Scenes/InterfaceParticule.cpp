#include "InterfaceParticule.h"
#include "..\..\Valkyrie\Moteur\Particlesystem.h"

#include <atlbase.h>
#include "..\..\Valkyrie\Moteur\Utils.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

CInterfaceParticule::CInterfaceParticule(CScene* pScene) : CInterface(pScene)
{

	m_dxfont = NULL;

}

CInterfaceParticule::~CInterfaceParticule()
{
}

bool CInterfaceParticule::Creation()
{
	m_pD3DSpt = m_pScene->GetMoteur()->GetMoteur3D()->GetSprites();
	m_LargeurFenetre=m_pScene->GetMoteur()->m_LargeurFenetre;	
	m_HauteurFenetre=m_pScene->GetMoteur()->m_HauteurFenetre;

	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &m_dxfont);

	if(!spriteViseur.chargerTextureFromFileEx(m_pD3DDevice, "..\\Sprites\\viseur3.png", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 255, 255), NULL, NULL))
		return false; 

	return true;
}




void CInterfaceParticule::afficher()
{

	m_pD3DSpt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing with transparency

	draw_Guns();

	AfficherFPS();
	AfficherNbParticules();
	m_pD3DSpt->End();    // end sprite drawing

	return;
}




void CInterfaceParticule::draw_Guns()
{
	RECT Part;

	m_pScene->GetMoteur()->GetMoteur3D()->GetDevice3D()->SetRenderState(D3DRS_WRAP0,0);

	SetRect(&Part,0, 0, 50,50);

	D3DXVECTOR3 center2(15.0f, 15.0f, 0.0f);
	D3DXVECTOR3 position2(m_LargeurFenetre/2, m_HauteurFenetre/2, 0.0f);
	m_pD3DSpt->Draw(spriteViseur.getDXTexture(), NULL, &center2, &position2, D3DCOLOR_ARGB(180,255, 255, 255));

}



void CInterfaceParticule::Release()
{
	SAFE_RELEASE(m_dxfont);
	spriteViseur.Release();
	m_dxfont = NULL;

}



void CInterfaceParticule::AfficherNbParticules()
{
	// create a RECT to contain the text
	RECT textbox;
	SetRect(&textbox, 20,20, 220, 60); 
	char my_string[22];
	sprintf(my_string,"%10i Particules",CParticleSystem::getNbParticule());

	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		my_string,
		strlen(my_string),
		&textbox,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}
