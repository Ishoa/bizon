
#ifndef CMESHCOLLISION_H
#define CMESHCOLLISION_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "Mesh.h"
#include "ObjetDeCollision.h"

class CMeshCollision : public CObjetDeCollision, public CMesh
{
  private:

	bool				m_bAffichageBoundingBox;
	bool				m_bAffichageBoundingSphere;

  public:

	CMeshCollision(LPDIRECT3DDEVICE9 pD3DDevice = NULL);
    ~CMeshCollision();

  public:

	D3DXMATRIXA16 GetWorldMatrix();

	bool ChargerMesh(LPSTR NomFichier);
	bool ChargerPolygonList();
	void Release();
	void RenduMesh(bool SaveState = true);
	void SetBoundingBox(bool baffichage){m_bAffichageBoundingBox = baffichage;}
	void SetBoundingSphere(bool baffichage){m_bAffichageBoundingSphere = baffichage;}
};

#endif // E_MESH_H
