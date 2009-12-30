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
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTemplate(m_pMoteur);
			break;

		case e_Scene_Test:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTest(m_pMoteur);
			break;

		case e_Scene_MipMapping:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneDemoMipMapping(m_pMoteur);
			break;

		case e_Scene_TeapotAndTracball:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTeapot(m_pMoteur);
			break;

		case e_Scene_MultipleVertexBuffers:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneVertexBuffer(m_pMoteur);
			break;

		case e_Scene_NormalMapping:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneBumpMapping(m_pMoteur);
			break;

		case e_Scene_Shader:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneShader(m_pMoteur);
			break;

		case e_Scene_SystemeSolaire:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneSystemSolaire(m_pMoteur);
			break;

		case e_Scene_Laby:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneLaby(m_pMoteur);
			break;

		case e_Scene_Terrain3D:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneTerrain(m_pMoteur);
			break;

		case e_Scene_ShadowVolume:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneShadowVolume(m_pMoteur);
			break;

		case e_Scene_ParticleSystem:
			// Cr�ation de la sc�ne � afficher
			m_eEtatRendu = e_RenduScene3D;
			m_pCurrentScene = new CSceneParticuleSystem(m_pMoteur);
			break;

		case e_Scene_EffectDX:
			// Cr�ation de la sc�ne � afficher
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

