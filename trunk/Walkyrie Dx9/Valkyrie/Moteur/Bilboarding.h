
#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Sprites3D.h"
#include "Scene.h"


class CBilboarding : public CSprite3D
{

	CScene* m_pScene;

	D3DXMATRIXA16 m_MatriceGeneral;	// Matrice de transformation 

public:
	CBilboarding(CScene* pScene);
	~CBilboarding();

	virtual void RenderSprite3D(bool SaveState = true);

};