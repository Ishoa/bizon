#include "stdafx.h"

#ifndef _ATLAS_
#include "Engine/Atlas/Atlas.h"
#endif

#ifndef _SCREEN_TEXT_
#include "Engine/Engine/ScreenText.h"
#endif

ScreenText::ScreenText()
{

}

ScreenText::~ScreenText()
{

}

HRESULT ScreenText::Create()
{
	// Atlas
	m_pAtlas = new Atlas;
	D_RETURN( m_pAtlas->Create( "..\\Data\\Atlas\\font.xml" ) );

	// Vertex Shader
	m_pVertexShader = new VertexShader("ScreenText.vsh");
	D_RETURN( m_pVertexShader->CompileAndCreate() );

	// Pixel Shader
	m_pPixelShader = new PixelShader("ScreenText.psh");
	D_RETURN( m_pPixelShader->CompileAndCreate() );

	// Vertex Layout
	m_pVertexLayout = new VertexLayout< ScreenTextTypeVertex >;
	D_RETURN( m_pVertexLayout->Create( m_pVertexShader ) );

	m_nVertex = MAX_CHAR * 4;

	// VertexBuffer
	m_pVertexBuffer = new VertexBufferEx;
	D_RETURN( m_pVertexBuffer->Create( sizeof(float) * 8, MAX_CHAR, NULL ) );

	return S_OK;
}

HRESULT ScreenText::Destroy()
{
	SAFE_DESTROY( m_pPixelShader );
	SAFE_DESTROY( m_pVertexShader );
	SAFE_DESTROY( m_pVertexLayout );
	SAFE_DESTROY( m_pVertexBuffer );
	return S_OK;
}

void ScreenText::Begin()
{
	m_nVertex = 0;
}

void ScreenText::End()
{
	float * pData = (float*)m_pVertexBuffer->Map();
	CopyMemory( pData, (void*)m_tSprite, sizeof(float) * m_nVertex * 8 );
	m_pVertexBuffer->Unmap();

	g_pDxDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	m_pVertexBuffer->Bind();
	m_pVertexLayout->Bind();
	m_pVertexShader->Bind();
	m_pPixelShader->Bind();
	g_pDevice->UnbindGeometryShader();

	g_pDxDeviceContext->Draw( m_nVertex, 0 );
}

HRESULT ScreenText::DrawText(const char * _str, unsigned int _x, unsigned int _y, const Color & _rgba)
{
	// [10;20] -> char size (texture)
	//unsigned int iIndex = m_nVertex % 4;
	//m_tSprite[ iIndex ] = 
	unsigned int iLength = (unsigned int)strlen( _str );
	for( unsigned int i = 0; i < iLength; ++i )
		AddCharacter( _str[i], i, _x, _y, _rgba );


	return S_OK;
}

void ScreenText::AddCharacter( char _c, unsigned int _Index, unsigned int _x, unsigned int _y, const Color & _rgba )
{
	float x0, x1, y0, y1;
	// TODO : retrieve window size
	float px = (float)(_x + _Index * m_pAtlas->GetIconWidth()) / 800.0f;
	float py = (float)_y / 600.0f;
	float fIconWidth = (float)m_pAtlas->GetIconWidth() / (float)m_pAtlas->GetAtlasWidth();
	float fIconHeight = (float)m_pAtlas->GetIconHeight() / (float)m_pAtlas->GetAtlasHeight();
	m_pAtlas->GetRectByIndex( (_c - 32), x0, y0, x1, y1 );
	
	// FACE 0
	{
		// VERTEX 0
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px;
		m_tSprite[m_nVertex * 8 + 1] = py;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x0;
		m_tSprite[ m_nVertex * 8 + 7 ] = y0;
		++m_nVertex;


		// VERTEX 2
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px + fIconWidth;
		m_tSprite[m_nVertex * 8 + 1] = py + fIconHeight;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x1;
		m_tSprite[ m_nVertex * 8 + 7 ] = y1;
		++m_nVertex;

		// VERTEX 1
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px + fIconWidth;
		m_tSprite[m_nVertex * 8 + 1] = py;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x1;
		m_tSprite[ m_nVertex * 8 + 7 ] = y0;
		++m_nVertex;
	}

	// FACE 1
	{
		// VERTEX 0
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px;
		m_tSprite[m_nVertex * 8 + 1] = py;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x0;
		m_tSprite[ m_nVertex * 8 + 7 ] = y0;
		++m_nVertex;

		// VERTEX 1
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px;
		m_tSprite[m_nVertex * 8 + 1] = py + fIconHeight;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x0;
		m_tSprite[ m_nVertex * 8 + 7 ] = y1;
		++m_nVertex;

		// VERTEX 2
		// X Y
		m_tSprite[m_nVertex * 8 + 0] = px + fIconWidth;
		m_tSprite[m_nVertex * 8 + 1] = py + fIconHeight;
		// R G B A
		m_tSprite[ m_nVertex * 8 + 2 ] = _rgba.Red();
		m_tSprite[ m_nVertex * 8 + 3 ] = _rgba.Green();
		m_tSprite[ m_nVertex * 8 + 4 ] = _rgba.Blue();
		m_tSprite[ m_nVertex * 8 + 5 ] = _rgba.Alpha();
		// U V
		m_tSprite[ m_nVertex * 8 + 6 ] = x1;
		m_tSprite[ m_nVertex * 8 + 7 ] = y1;
		++m_nVertex;
	}

}