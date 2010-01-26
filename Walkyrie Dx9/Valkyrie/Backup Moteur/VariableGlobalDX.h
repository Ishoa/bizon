
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dinput.h>


#include "DXDataBase.h"

extern CDXDataBase DxDataBase;

#define D3DCOLOR_R(Couleur)  ((Couleur >> 16) & 0xFF)
#define D3DCOLOR_G(Couleur)  ((Couleur >> 8) & 0xFF)
#define D3DCOLOR_B(Couleur)  (Couleur & 0xFF)
#define D3DCOLOR_A(Couleur)  ((Couleur >> 24) & 0xFF)
#define D3DCOLOR_Gris(Couleur) ((D3DCOLOR_R(Couleur)+D3DCOLOR_G(Couleur)+D3DCOLOR_B(Couleur))/3)

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }