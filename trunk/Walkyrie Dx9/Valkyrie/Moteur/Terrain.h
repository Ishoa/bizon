
// Classe pour la gestion du Terrain en 3D

#pragma once
#include "..\Moteur\VariableGlobale.h"
#include "..\Moteur\Scene.h"
#include "Texture.h"

class CScene;

// Type de vertex utilisé pour le terrain
struct CUSTOMVERTEX
{
	FLOAT x, y, z;		// poition
	FLOAT tx1, ty1;		// coordonnées textures 1
	FLOAT tx2, ty2;		// coordonnées textures 2
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_TEX2)

// Macro pour utiliser des tableau crée dynamiquement
#define TMapHauteur(x, y) (m_MapHauteur[x+y*m_NbPointX])
#define TMapHauteurLisser(x, y) (pMapHauteurLisser[x+y*m_NbPointX])


class CTerrain
{
public:
	CScene* m_pScene;						// pointeur sur la scène
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// pointeur sur l'objet Direct3DDevice

	LPDIRECT3DVERTEXBUFFER9 m_pVBTerrain;	// pointeur sur le vertex buffer du terrain
	CUSTOMVERTEX* m_pVertexTerrain;			// pointeur sur le tableau de vertex

	float* m_MapHauteur;					// pointeur le tableau d'altitude du terrain

	int m_NbPointX;							// nombre de vertex en X du terrain
	int m_NbPointY;							// nombre de vertex en Y du terrain
	int m_NbMailleX;						// nombre de quad en X du terrain
	int m_NbMailleY;						// nombre de quad en Y du terrain
	int m_NbVertex;							// nombre de vertex total du buffer
	int m_NbTriangles;						// nombre de triangles total pour le terrain
	int m_OffsetVertex;						// offset pour arrivé à la fin des vertes pour le terrain

	float m_fCenterX;
	float m_fCenterZ;

	CLPTexture m_pTxHerbe;			// pointeur sur la texture d'herbe
	CLPTexture m_pTxTerre;			// pointeur sur la texture de terre
	CLPTexture m_pTxDensiteTerre;	// pointeur sur la texture de densité de terre
	CLPTexture m_pTxEau;			// pointeur sur la texture d'eau
	CLPTexture m_pTxDensiteEau;		// pointeur sur la texture de densité d'eau

	float m_CoefHauteur;					// coéficient pour calculé la hauteur du terrain
	
	float m_HauteurEau;						// hauteur de l'eau
	float m_TranslationEau;					// transation de la texture d'eau
	D3DXMATRIXA16 m_MatTransEau;			// matrice de transformation de l'eau
	
	float m_RecouvrementTexure;				// nombre de quad par texture
	

	CTerrain();
	CTerrain(CScene* pScene);
	~CTerrain();

	bool CreationTerrain(char* FichierMapTerrain, float fDecalageHauteur);

	bool ChargerTerrain();
	bool SetSurface(char* FichierTextureSurface);
	bool SetTerre(char* FichierDensiteTerre,char * FichierTextureTerre);
	bool SetEaux(char* FichierDensiteEau,char * FichierTextureEau, float fHauteurEau);

	void Release();
	void RenduTerrain();

	bool ChargerMapHauteur(char* FichierMapTerrain, float fDecalageHauteur);
	void LisserMapHauteur();
	bool CreationMaillage();
	

	void Animation(double TempsEcouler);
	float GetHauteur(float x, float y);
	LPDIRECT3DDEVICE9 Get3DDevice(){return m_pD3DDevice;}
	int GetNbPointX(){return m_NbPointX;}
	int GetNbPointY(){return m_NbPointY;}
	CCamera* GetCameraCourante(){return m_pScene->GetCameraCourante();}
};