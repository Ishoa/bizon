#ifndef _VIEW_PORT_
#define _VIEW_PORT_

#include <d3d11.h>

class Device3D;

class ViewPort
{
private:
	D3D11_VIEWPORT	m_oViewPort;
public:
	ViewPort();
	~ViewPort();

	HRESULT Create(float x, float y, float width, float height, float zMin, float zMax);
	HRESULT Destroy();
	void	Resize(float _uWidth, float _uHeight);

	inline D3D11_VIEWPORT GetViewPort() const { return m_oViewPort; }

};

#endif // _VIEW_PORT_