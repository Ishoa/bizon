#include "SceneTemplate.h"

#define NB_STAGE 2


CSceneTemplate::CSceneTemplate(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 1;
	m_pInterface = NULL;
}

CSceneTemplate::~CSceneTemplate()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On v�rifier que la carte graphique dispose des fonctions n�c�ssaire au rendu
bool CSceneTemplate::ConfirmeFonction3D(D3DCAPS9* pCaps)
{
	if(!(pCaps->TextureCaps & D3DPTEXTURECAPS_PROJECTED))
	{
		Console << "Le peripherique ne permet pas la projection de texture" << endl;
		return false;
	}
	if(pCaps->MaxTextureWidth < 1024 || pCaps->MaxTextureHeight < 1024)
	{
		Console << "Le peripherique ne permet pas l'utilisation de texture de taille egale a 1024" << endl;
		return false;
	}
	if(!(pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP))
	{
		Console << "Le peripherique ne permet pas l'utilisation du bump mapping" << endl;
		return false;
	}
	if(pCaps->MaxSimultaneousTextures < 2)
	{
		Console << "Le peripherique ne permet pas l'utilisation de 2 etages de texture" << endl;
		return false;
	}
	if(pCaps->MaxUserClipPlanes < 1)
	{
		Console << "Le peripherique ne permet pas l'utilisation de plan de clipping" << endl;
		return false;
	}
	return true;
}

// Initialisation de la sc�ne 3D
bool CSceneTemplate::Initialisation()
{

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(100.0f, 10.0f, 0.0f));

	// de Base la lumiere est activ�
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pInterface = new CInterface(this);
	return true;


}

// Cr�ation des objets 3D de la sc�ne
bool CSceneTemplate::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);


	// Initialisation de la lumi�re 1 et activation

	// Calcule des matrices de la cam�ra
	m_pCameraCourante->SetMatrice();


	if(!m_pInterface->Creation())
		return false;
	return true;
}

// Destruction des resource utilis� par les objets
void CSceneTemplate::DestructionObjet()
{
	m_pInterface->Release();
}

// Destruction des objets
void CSceneTemplate::Destruction()
{

}

// Rendu 3D de la sc�ne
void CSceneTemplate::Rendu3D()
{

	m_pInterface->afficher();
}

// Calculs des animations de la sc�ne
void CSceneTemplate::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la cam�ra dans un �tat normal
	GetCameraCourante()->SetMatrice();
}

// Test des entr�es des p�riph�riques
void CSceneTemplate::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{
	// On test si on a chang� un param�tre de qualit�
	if(KEYDOWN(DIK_F1))
	{
		m_QualiterFiltrage = 0;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F2))
	{
		m_QualiterFiltrage = 1;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F3))
	{
		m_QualiterFiltrage = 2;
		SetFiltreTexture(m_QualiterFiltrage);
	}
}

// Appliqu� les param�tres de filtrage de texture
void CSceneTemplate::SetFiltreTexture(int Type)
{
	switch(Type)
	{
		// Filtrage Bilin�aire
	case 0:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		}

		break;

		// Filtrage Tilin�aire
	case 1:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		break;

		// Filtrage Anisotropic
	case 2:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		}
		break;
	}
}
