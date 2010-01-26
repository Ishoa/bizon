#include "Interface.h"
#include <atlbase.h>
#include "Utils.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

CInterface::CInterface(CScene* pScene)
{
	m_pScene = pScene;
	m_dxfont = NULL;
	m_pD3DDevice = m_pScene->Get3DDevice();
	

	m_fps = 0.0f;
}

CInterface::~CInterface()
{
}

bool CInterface::Creation()
{
	m_pD3DSpt = m_pScene->GetMoteur()->GetMoteur3D()->GetSprites();
	m_LargeurFenetre=m_pScene->GetMoteur()->m_LargeurFenetre;	
	m_HauteurFenetre=m_pScene->GetMoteur()->m_HauteurFenetre;

	D3DXCreateFont(m_pD3DDevice, 20, 0, FW_BOLD, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		"Arial", &m_dxfont);

	return true;
}


// this draws a portion of the specified texture
void CInterface::DrawTextureSprites(LPDIRECT3DTEXTURE9 texture, RECT texcoords, float x, float y, int a)
{
    D3DXVECTOR3 center(0.0f, 0.0f, 0.0f), position(x, y, 0.0f);
	m_pD3DSpt->Draw(texture, &texcoords, &center, &position, D3DCOLOR_ARGB(a,255, 255, 255));

    return;
}


void CInterface::afficher()
{

   	m_pD3DSpt->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing with transparency

	AfficherFPS();

	m_pD3DSpt->End();    // end sprite drawing

	return;
}


void CInterface::AfficherFPS()
{
	// create a RECT to contain the text
	RECT textbox;
	SetRect(&textbox, m_LargeurFenetre/2,20, m_LargeurFenetre/2+100, 60); 

	// changement de la transformation
	UItime = timeGetTime();
	//Console<<" FPS : "<<DXOutils::GetFPS()<<endl;
	m_fps = DXOutils::GetFPS();
	char my_string[16];
	sprintf(my_string,"%10.2f FPS",m_fps);

	// draw the Hello World text
	m_dxfont->DrawTextA(NULL,
		my_string,
		strlen(my_string),
		&textbox,
		DT_RIGHT,
		D3DCOLOR_ARGB(255, 255, 0, 0));
}


void CInterface::Release()
{
	m_dxfont->Release();
	m_dxfont = NULL;
}