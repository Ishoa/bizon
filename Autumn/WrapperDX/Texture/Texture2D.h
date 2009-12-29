#ifndef _TEXTURE2D_
#define _TEXTURE2D_

#ifndef _TEXTURE_
#include "WrapperDx/Texture/Texture.h"
#endif

class Texture2D : public Texture
{
protected:
	char						m_strFilename[64];
	ID3D11Texture2D				* m_pTexture;
public:
	Texture2D();
	Texture2D(const char * _strFilename);
	virtual ~Texture2D();

	void SetName(const char * _strFilename);

	virtual HRESULT Create();
	virtual HRESULT Destroy();
};

#endif