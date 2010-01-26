
// Classe pour la gestion d'un model 3D

#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "DXPileEtats.h"

class CScene;

class CMesh
{

public:

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	LPD3DXMESH              m_pMesh;			// Pointeur sur le Mesh
	D3DMATERIAL9*           m_pMeshMaterielles;	// Pointeur sur les matérielles
	LPDIRECT3DTEXTURE9*     m_pMeshTextures;	// Pointeur sur les textures
	DWORD                   m_NumMaterielles;	// Nombre de matérielle
	
	CDXPileEtats*		DXEtats;

	LPSTR               m_pFilename;

	float               m_fRX;
	float               m_fRY;
	float               m_fRZ;

	float				m_fTX;
	float				m_fTY;
	float				m_fTZ;

	float				m_fScaleX;
	float				m_fScaleY;
	float				m_fScaleZ;

	bool                m_bIsAlphaBlended;


	CMesh(LPDIRECT3DDEVICE9 pD3DDevice= NULL);
	//CMesh();

	~CMesh();

	float GetRotationX    ();
	void  SetRotationX    ( float Value );
	float GetRotationY    ();
	void  SetRotationY    ( float Value );
	float GetRotationZ    ();
	void  SetRotationZ    ( float Value );
	void  SetRotationXYZ  ( float fXValue, float fYValue, float fZValue);

	float GetScaleX    ();
	void  SetScaleX    ( float Value );
	float GetScaleY    ();
	void  SetScaleY    ( float Value );
	float GetScaleZ    ();
	void  SetScaleZ    ( float Value );
	void  SetScaleXYZ  ( float fXValue, float fYValue, float fZValue);


	float GetTranslationX ();
	void  SetTranslationX ( float Value );
	float GetTranslationY ();
	void  SetTranslationY ( float Value );
	float GetTranslationZ ();
	void  SetTranslationZ ( float Value );
	void  SetTranslationXYZ ( float fXValue, float fYValue, float fZValue);


	virtual bool ChargerMesh(LPSTR NomFichier);
	virtual void Release();
	virtual void RenduMesh(bool SaveState = true);
	virtual bool RestoreMesh();


	virtual D3DXMATRIXA16 GetWorldMatrix();
	void SetWorldMatrix(D3DXMATRIXA16* ObjectMatrix);

	void  SetAlphaBlending( bool Value );
	bool  IsAlphaBlended  ();

protected:
	void SetAlphaBlending    ();
	void PushDeviceProperties();
	void PopDeviceProperties ();

};
