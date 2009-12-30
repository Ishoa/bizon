
// Classe pour la gestion de la végétation

#pragma once
#include "Terrain.h"

class CTerrain;

// Type de vertex utilisé pour la végétation
struct CUSTOMVERTEXV
{
    FLOAT x, y, z;
	FLOAT tx1, ty1;
};

#define D3DFVF_CUSTOMVERTEXV (D3DFVF_XYZ | D3DFVF_TEX1)



class CVegetation
{
public:
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// pointeur sur l'objet Direct3DDevice
	CTerrain* m_pTerrain;			// pointeur sur le Terrain

	LPDIRECT3DVERTEXBUFFER9 m_pVB;	// pointeur sur le vertex buffer de la végétation

	CLPTexture m_pTxFleur;	// pointeur sur la texture d'une fleurs
	CLPTexture m_pTxPlante;	// pointeur sur la texture d'une plante
	CLPTexture m_pTxArbre1;	// pointeur sur la texture d'un arbre
	CLPTexture m_pTxArbre2;	// pointeur sur la texture d'un arbre
	CLPTexture m_pTxArbre3;	// pointeur sur la texture d'un arbre

	D3DXMATRIXA16 m_MatriceGeneral;	// Matrice de transformation 

	CUSTOMVERTEXV* m_pVertex;		// pointeur sur le tableau de vertex
	int m_NbVertex;					// nombre de vertex total du buffer

	int m_NbArbre;					// nombre d'arbre total
	int m_NbArbre1;					// nombre d'arbre de type 1
	int m_NbArbre2;					// nombre d'arbre de type 2
	int m_NbArbre3;					// nombre d'arbre de type 3
	int m_NbPlante;					// nombre de plante
	int m_NbFleur;					// nombre de fleur

	D3DXVECTOR3* m_pArbre;			// coordonnées des arbres
	D3DXVECTOR3* m_pPlante;			// coordonnées des plante
	D3DXVECTOR3* m_pFleur;			// coordonnées des fleurs

	char* m_pFichierFleur;
	char* m_pFichierPlante;
	char* m_pFichierArbreType1;
	char* m_pFichierArbreType2;
	char* m_pFichierArbreType3;

	CVegetation();
	CVegetation(CTerrain* pTerrain);
	~CVegetation();

	bool Creation(float Taille = 5.0f);

	bool SetMapFleur(char* FichierMapFleur, int nNbFleur, char * pFichierFleur);
	bool SetMapPlante(char* FichierMapPlante, int nNbPlante, char * pFichierPlante);
	bool SetMapArbre(char* FichierMapArbre, int nNbArbre, char * pFichierArbreType1,char * pFichierArbreType2 = NULL,char * pFichierArbreType3 = NULL); 


	bool CreationVertex();
	bool ChargerVegetation();
	void Release();
	void RenduVegetation();
};