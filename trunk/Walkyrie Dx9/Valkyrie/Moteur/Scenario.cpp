#include "Scenario.h"


CScenario::CScenario()
{
	m_pCurrentScene = NULL;
	m_pMoteur = CMoteur::GetCurrent();
}


CScenario::~CScenario()
{
	if(m_pCurrentScene)
		delete m_pCurrentScene;
}

bool CScenario::Creation()
{
	return Init();
}


bool CScenario::Init()
{
	return Reset();
}


bool CScenario::Reset()
{
	if(m_pCurrentScene != NULL)
	{
		m_pCurrentScene->DestructionFinal();
		delete m_pCurrentScene;
		m_pCurrentScene = NULL;
	}

	// Création de la scène à afficher
	m_eEtatRendu = e_RenduScene3D;
	m_pCurrentScene = new CScene(m_pMoteur);

	if(m_pCurrentScene == NULL)
		return false;

	if(!m_pCurrentScene->Creation())
		return false;

	m_pCurrentScene->SetRatioFenetre((float)m_pMoteur->GetLargeurFenetre()/(float)m_pMoteur->GetHauteurFenetre());
	
	return true;
}


void CScenario::Rendu()
{
	switch(m_eEtatRendu)
	{
		case e_RenduScene3D:
			RenduScene3D();
			break;

		case e_RenduCinematic:
			RenduCinematic();
			break;

		default:
			break;
	}

// 	SetWindowPos(CMoteur::GetCurrent()->GetFenetre(), NULL, 0, 0, 1024, 1024, 
// 		SWP_NOMOVE | SWP_NOOWNERZORDER);

}
void CScenario::DestructionFinal()
{
	if(m_pCurrentScene)
		m_pCurrentScene->DestructionFinal();
}

void CScenario::DestructionObjet()
{
	if(m_pCurrentScene)
		m_pCurrentScene->DestructionObjet();
}

void CScenario::RenduScene3D()
{
	if(m_pCurrentScene)
		m_pCurrentScene->Rendu();
}

void CScenario::RenduCinematic()
{




}

void CScenario::SetRatioFenetre(float fRatio)
{
	if(m_pCurrentScene)
		m_pCurrentScene->SetRatioFenetre(fRatio);
};

void CScenario::CreationObjet()
{
	if(m_pCurrentScene)
		m_pCurrentScene->CreationObjet();
}