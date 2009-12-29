#ifndef _VERTEX_LAYOUT_
#define _VERTEX_LAYOUT_

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

class Device3D;
class VertexShader;
class Effect;
class TypeVertex;

template < class Vertextype >
class VertexLayout
{
private:
	ID3D11InputLayout			* m_pVertexLayout;
	Vertextype					* m_pTypeVertex;

public:
	VertexLayout();
	~VertexLayout();

 	HRESULT		Create(VertexShader * pVsh);
// 	HRESULT		Create(Effect * _pEffect, unsigned int _uTechnique = 0, unsigned int _uPass = 0);
	HRESULT		Destroy();
	void		Bind();
};

template < class Vertextype >
VertexLayout< Vertextype >::VertexLayout()
: m_pVertexLayout(NULL)
, m_pTypeVertex(NULL)
{

}

template < class Vertextype >
VertexLayout< Vertextype >::~VertexLayout()
{

}

template < class Vertextype >
HRESULT VertexLayout< Vertextype >::Create(VertexShader * pVsh)
{
	if( ! pVsh )
		return E_FAIL;

	m_pTypeVertex = new Vertextype;
	D_RETURN( m_pTypeVertex->Create() );

	D_RETURN( g_pDxDevice->CreateInputLayout( m_pTypeVertex->GetLayout(), m_pTypeVertex->GetLayoutSize(), pVsh->GetShaderCode(), pVsh->GetShaderSize(), &m_pVertexLayout ) );

	return S_OK;

}

template < class Vertextype >
HRESULT VertexLayout< Vertextype >::Destroy()
{
	SAFE_DESTROY( m_pTypeVertex );
	SAFE_RELEASE( m_pVertexLayout );

	return S_OK;
}

template < class Vertextype >
void VertexLayout< Vertextype >::Bind()
{
	g_pDxDeviceContext->IASetInputLayout( m_pVertexLayout );
}



#endif // _VERTEX_LAYOUT_