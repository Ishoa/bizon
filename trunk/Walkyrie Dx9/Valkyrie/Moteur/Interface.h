// Classe pour la gestion de l'application
#ifndef _CINTERFACE_H
#define _CINTERFACE_H

#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Moteur3D.h"
#include "texture.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class CMoteur3D;

class CInterface
{
	public:

		CScene* m_pScene;
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		LPD3DXSPRITE m_pD3DSpt;

		int	m_LargeurFenetre;			// Largeur de la fenêtre
		int	m_HauteurFenetre;			// Hauteur de la fenêtre

		double m_fps;

		LPD3DXFONT m_dxfont;    // the pointer to the font object

		CInterface(CScene* pScene);
		~CInterface();

		virtual bool Creation();
		virtual void afficher();
		void AfficherFPS();

		virtual void Release();
	protected:
		void CInterface::DrawTextureSprites(CLPTexture texture, RECT texcoords, float x, float y, int a);
	
};

#endif _CINTERFACE_H