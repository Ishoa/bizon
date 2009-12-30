#include "MyScenario.h"
#include "SceneTest.h"
#include "SceneTerrain.h"
#include "SceneTemplate.h"
#include "SceneTeapot.h"
#include "SceneVertexBuffers.h"
#include "SceneSystemeSolaire.h"
#include "SceneDemoMipMapping.h"
#include "SceneBumpMapping.h"
#include "SceneLaby.h"
#include "SceneShader.h"
#include "SceneTest.h"
#include "SceneShadowVolume.h"
#include "ScenePaticuleSystem.h"
#include "SceneEffectDx.h"

CMyScenario::CMyScenario():CScenario()
{

	m_pCurrentScene = NULL;

}

CMyScenario::~CMyScenario()
{

}


bool CMyScenario::Init()
{


	m_eCurrentScene = e_Scene_Test;
	return Reset();

};

bool CMyScenario::InitScene(EScene eScene)
{

	if(m_pCurrentScene != NULL)
	{
		m_pCurrentScene->DestructionFinal();
		delete m_pCurrentScene;
		m_pCurrentScene = NULL;
	}

	switch(eScene)
	{
		case e_Scene_Basique:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTemplate(m_pMoteur);
			break;

		case e_Scene_Test:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTest(m_pMoteur);
			break;

		case e_Scene_MipMapping:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneDemoMipMapping(m_pMoteur);
			break;

		case e_Scene_TeapotAndTracball:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTeapot(m_pMoteur);
			break;

		case e_Scene_MultipleVertexBuffers:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneVertexBuffer(m_pMoteur);
			break;

		case e_Scene_NormalMapping:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneBumpMapping(m_pMoteur);
			break;

		case e_Scene_Shader:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneShader(m_pMoteur);
			break;

		case e_Scene_SystemeSolaire:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneSystemSolaire(m_pMoteur);
			break;

		case e_Scene_Laby:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneLaby(m_pMoteur);
			break;

		case e_Scene_Terrain3D:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTerrain(m_pMoteur);
			break;

		case e_Scene_ShadowVolume:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneShadowVolume(m_pMoteur);
			break;

		case e_Scene_ParticleSystem:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneParticuleSystem(m_pMoteur);
			break;

		case e_Scene_EffectDX:
			// Création de la scène à afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneEffectDX(m_pMoteur);
			break;

		case e_Scene_Cinematique:
			break;

		default:break;

	}

	if(m_pCurrentScene == NULL)
		return false;

	if(!m_pCurrentScene->Creation())
		return false;

	m_eCurrentScene = eScene;

	return true;
}

bool CMyScenario::Reset()
{

	InitScene(m_eCurrentScene);
	m_pCurrentScene->SetRatioFenetre((float)m_pMoteur->GetLargeurFenetre()/(float)m_pMoteur->GetHauteurFenetre());

	return true;
}

