
#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#ifndef _CDXETATS_
#define _CDXETATS_

class CDXPileEtats
{

	LPDIRECT3DDEVICE9 m_pD3DDevice;

	DWORD               m_AdressU;
	DWORD               m_AdressV;
	DWORD				m_ZEnable;
	DWORD               m_ZWriteEnable;
	DWORD               m_AlphaBlendEnable;
	DWORD               m_AlphaTestEnable;
	DWORD               m_SrcBlend;
	DWORD               m_DestBlend;
	DWORD				m_Light;

public:

	CDXPileEtats(LPDIRECT3DDEVICE9 m_pD3DDevice = NULL);
	~CDXPileEtats();

	LPDIRECT3DDEVICE9 GetDevice3D(){return m_pD3DDevice;}
	bool PushDeviceProperties();
	bool PopDeviceProperties ();
};

#endif // _CDXETATS_