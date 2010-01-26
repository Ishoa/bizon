#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Mesh.h"

class CPanorama : public CMesh
{

public :

	CScene* m_pScene;
	
	D3DXMATRIXA16 m_MatriceView;	// matrice de transformation de Vision
	D3DXMATRIXA16 m_MatriceWorld;	// matrice de transformation du monde

	CPanorama(CScene* pScene);
	~CPanorama();

	void RenduMesh(bool SaveState = true);
};
