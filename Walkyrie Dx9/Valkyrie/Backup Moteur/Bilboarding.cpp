#include "Bilboarding.h"


CBilboarding::CBilboarding(CScene* pScene):CSprite3D(pScene->Get3DDevice())
{
	m_pScene = pScene;
	D3DXMatrixIdentity(&m_MatriceGeneral);
}


CBilboarding::~CBilboarding()
{


}

void CBilboarding::RenderSprite3D(bool SaveState)
{

	// On récupère la caméra courante
	CCamera* pCamera = m_pScene->GetCameraCourante();
	D3DXVECTOR3 Position  = pCamera->GetPosition();
	D3DXVECTOR3 Direction = pCamera->GetCible() - Position;
	float Angle = 0.0f;
	float Rotation = 0.0f;


	// On calcule l'angle pour orienter la végétation face à la caméra
	if(Direction.x < 0.0f)
		Rotation = -atanf(Direction.z/Direction.x)+D3DX_PI/2;
	else
		Rotation = -atanf(Direction.z/Direction.x)-D3DX_PI/2;
	
	SetRotationY(Rotation+D3DX_PI);

	// On initialise le type de test sur l'alpha
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

	// Seuil de transparencre de la texture
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x000000A0);

	CSprite3D::RenderSprite3D(SaveState);

	// Initialisation des stages de rendu (opération sur les textures)
	m_pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP,    D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState (1, D3DTSS_COLOROP,   D3DTOP_DISABLE);

}