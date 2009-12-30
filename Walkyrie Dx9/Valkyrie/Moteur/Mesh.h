
// Classe pour la gestion d'un model 3D

#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Scene.h"
#include "DXPileEtats.h"
#include "Texture.h"

class CScene;

class CMesh
{

public:

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	// Mesh
	LPD3DXMESH              m_pMesh;			// Pointeur sur le Mesh
	D3DMATERIAL9*           m_pMeshMaterielles;	// Pointeur sur les matérielles
	CLPTexture*				m_pMeshTextures;	// Pointeur sur les textures
	DWORD                   m_NumMaterielles;	// Nombre de matérielle

	// Mesh Low Detail
	LPD3DXMESH              m_pMeshLowDetail;			// Pointeur sur le Mesh Low Detail
	D3DMATERIAL9*           m_pMeshMateriellesLowDetail;	// Pointeur sur les matérielles Low Detail
	DWORD                   m_NumMateriellesLowDetail;	// Nombre de matérielle Low Detail

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

	D3DXVECTOR3 m_vMaxMesh;
	D3DXVECTOR3 m_vMinMesh;

	// Cache of data in m_pMesh for easy access
// 	IDirect3DVertexBuffer9* m_pVB;
// 	IDirect3DIndexBuffer9* m_pIB;
	IDirect3DVertexDeclaration9* m_pDecl;

	DWORD m_dwNumVertices;
	DWORD m_dwNumFaces;
	DWORD m_dwBytesPerVertex;
	bool m_bUseMaterials;
	CScene* m_pScene;


	CMesh(CScene* pScene);
	//CMesh();

	~CMesh();

	LPD3DXMESH  GetDXMesh(){return m_pMesh;}
	LPDIRECT3DDEVICE9 GetDevice3D(){return m_pD3DDevice;}

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


	virtual bool ChargerMeshLowDetail(LPSTR NomFichier);
	virtual bool ChargerMeshLowDetail(LPD3DXMESH pMeshLow,D3DMATERIAL9* pMeshMateriellesLowDetail,DWORD NumMateriellesLowDetail);


	HRESULT CMesh::SetVertexDecl( const D3DVERTEXELEMENT9* pDecl,
									bool bAutoComputeNormals, bool bAutoComputeTangents,
									bool bSplitVertexForOptimalTangents );

	HRESULT     SetFVF(DWORD dwFVF );

	virtual void Release();

	
	virtual void SimpleRenduMesh(bool textured = false);

	virtual void RenduMeshSampler(bool SaveState = true);

	// Rendering
	virtual void      RenduMesh( bool bDrawOpaqueSubsets = true,
						bool bDrawAlphaSubsets = true );

	virtual void     RenduMesh( ID3DXEffect* pEffect,
						D3DXHANDLE hTexture = NULL,
						D3DXHANDLE hDiffuse = NULL,
						D3DXHANDLE hAmbient = NULL,
						D3DXHANDLE hSpecular = NULL,
						D3DXHANDLE hEmissive = NULL,
						D3DXHANDLE hPower = NULL,
						bool bDrawOpaqueSubsets = true,
						bool bDrawAlphaSubsets = true );

	virtual void RenduMeshLowDetail(bool SaveState = true);
	//virtual void RenduMeshEffect(LPD3DXEFFECT  Fx);

	virtual bool RestoreMesh();


	virtual D3DXMATRIXA16 GetWorldMatrix();
	void SetWorldMatrix(D3DXMATRIXA16* ObjectMatrix);

	void  SetAlphaBlending( bool Value );
	bool  IsAlphaBlended  ();

	void UseMeshMaterials( bool bFlag ){m_bUseMaterials = bFlag;}

	virtual bool BoundingBoxIsVisibleByCamera();
	virtual bool BoundingSphereIsVisibleByCamera();

protected:
	void SetAlphaBlending    ();

};
