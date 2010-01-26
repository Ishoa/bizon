#ifndef _CLPTEXTURE_
#define _CLPTEXTURE_


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"


class CLPTexture
{
	private:
		LPDIRECT3DTEXTURE9	m_pTexture;

	public:

		CLPTexture();
	
		~CLPTexture();

		bool chargerTexture(LPDIRECT3DDEVICE9 pD3DDevice, char* pUrl);

		CLPTexture& operator= (const CLPTexture& pText);
		LPDIRECT3DTEXTURE9 getDXTexture()const {return m_pTexture;}

		int getNbInstanceTexture();

		void Release();

};

#endif _CLPTEXTURE_