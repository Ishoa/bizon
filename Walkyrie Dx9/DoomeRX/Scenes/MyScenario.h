#include "Scenario.h"


class CMyScenario : public CScenario
{
public:

	enum EScene
	{	
		e_Scene_Basique = 0,
		e_Scene_Test,
		e_Scene_MipMapping,
		e_Scene_TeapotAndTracball,
		e_Scene_MultipleVertexBuffers,
		e_Scene_NormalMapping,
		e_Scene_Shader,
		e_Scene_SystemeSolaire,
		e_Scene_Laby,
		e_Scene_Terrain3D,
		e_Scene_ShadowVolume,
		e_Scene_ParticleSystem,
		e_Scene_EffectDX,
		e_Scene_Cinematique,

	};

protected:
	EScene m_eCurrentScene;

public:
	CMyScenario();
	~CMyScenario();

	bool InitScene(EScene eScene);
	void SetScene(EScene eScene){m_eCurrentScene = eScene;}

	bool Init();
	bool Reset();
};