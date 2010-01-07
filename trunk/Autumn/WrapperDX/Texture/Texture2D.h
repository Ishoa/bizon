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
	unsigned int				m_iWidth;
	unsigned int				m_iHeight;
public:
	Texture2D();
	Texture2D(const char * _strFilename);
	virtual ~Texture2D();

	void SetName(const char * _strFilename);

	virtual HRESULT Create(bool _bUseTexturePath = true);
	virtual HRESULT Destroy();

	inline unsigned int GetWidth() const { return m_iWidth; }
	inline unsigned int GetHeight() const { return m_iHeight; }
};

#endif