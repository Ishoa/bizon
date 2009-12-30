#include "BattleFly3D.h"

CSceneBattleFly::CSceneBattleFly(CMoteur* pMoteur):CScene(pMoteur)
{
	m_pAvion = NULL;
	m_pCiel = NULL;
}

CSceneBattleFly::~CSceneBattleFly()
{
	Console << "Deconstructeur CSceneTerrain" << endl;
}

// Initialisation de la scène 3D
bool CSceneBattleFly::Initialisation()
{

	m_bRenduFilDeFer = false;

	m_Lumiere.SetDirectionnelle(&D3DXVECTOR3(2.0f, -1.0f, 0.0f));
	m_Lumiere.SetAmbiante(0.6f, 0.6f, 0.6f);
	m_Lumiere.SetDiffuse(1.0f, 1.0f, 1.0f);

	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0);

	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(64.0f, 16.0f, 0.0f));
	m_pCamera= new CCamera(this);
	m_pCamera->Creation();

	m_pAvion = new CMesh(this);
	m_pCiel = new CMesh(this);

	SetCamera(m_pCameraParDefaut);
	
	return true;
}

// Création des objets 3D de la scène
bool CSceneBattleFly::CreationObjet()
{
	// Chargement de l'avion 
	if(!m_pAvion->ChargerMesh("..\\Donnees\\airplane 2.x"))return false;
	// Chargement de la skybox
	if(!m_pCiel->ChargerMesh("..\\Donnees\\skybox2.x"))return false;

	// Applique les paramètre de la caméras
	m_pCameraCourante->SetMatrice();
	// Applique le mode de rendu des polygones
	if(m_bRenduFilDeFer) 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else 
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// On desactive le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// On initialise la lumière 0 (pour l'avion)
	SetLumiere(0, &m_Lumiere, true);
	return true;
}

// Destruction des resource utilisé par les objets
void CSceneBattleFly::DestructionObjet()
{
	m_pAvion->Release();
	m_pCiel->Release();
}

// Rendu 3D de la scène
void CSceneBattleFly::Rendu3D()
{
	// Initialisation de la matrice générale avec la matrice identité
	D3DXMatrixIdentity(&m_MatriceGeneral);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatriceGeneral);

	// On récupère la matrice de vision pour placer la camera au centre de la skybox
	m_MatriceGeneral = m_pCameraCourante->GetMatriceVision();
	m_MatriceGeneral._41 = m_MatriceGeneral._42 = m_MatriceGeneral._43 = 0.0f;
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceGeneral);

	// On desactive le ZBuffer
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// On active D3DTADDRESS_CLAMP pour une bonne jointure des textures de la sky box
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP);

	// Rendu de la skybox
	m_pCiel->RenduMeshSampler();

	// On récupère la matrice de vision pour la remètre dans son état initial 
	m_MatriceGeneral = m_pCameraCourante->GetMatriceVision();
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_MatriceGeneral);

	// On réactive le ZBuffer
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);

	// On applique la matrice de traformation pour placer l'avion
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_MatTransAvion);

	// On active le calcule de la lumière
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Rendu de l'avion
	m_pAvion->RenduMeshSampler();


}

// Calculs des animations de la scène
void CSceneBattleFly::Animation(double TempsEcouler, double DeltaTemps)
{

}

// Test des entrées des périphériques
void CSceneBattleFly::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris)
{

}


// Début du rendu 3D de la scène
void CSceneBattleFly::DebutRendu3D()
{
	if(m_bRenduFilDeFer)
	{
		// On efface le buffer image et le zbuffer
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		m_pD3DDevice->BeginScene();
	}
	else
	{
		// On efface le zbuffer
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		m_pD3DDevice->BeginScene();
	}
}
