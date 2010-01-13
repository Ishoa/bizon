#include "stdafx.h"

#ifndef _ATLAS_
#include "Engine/Atlas/Atlas.h"
#endif



Atlas::Atlas()
: m_pTexture(NULL)
, m_nCols(0)
, m_nRows(0)
, m_iIconWidth(0)
, m_iIconHeight(0)
{

}

Atlas::~Atlas()
{

}

HRESULT Atlas::Create( const char * _name )
{
	// xml file
	TiXmlDocument doc( _name );
	bool bLoadOkay = doc.LoadFile();
	if( ! bLoadOkay )
		return E_FAIL;

	TiXmlElement * pElement = doc.FirstChildElement("Icon");
	if( pElement )
	{
		if( pElement->QueryIntAttribute("width", &m_iIconWidth) == TIXML_NO_ATTRIBUTE )
			return E_FAIL;
		if( pElement->QueryIntAttribute("height", &m_iIconHeight) == TIXML_NO_ATTRIBUTE )
			return E_FAIL;
	}
	else
	{
		return E_FAIL;
	}
	pElement = doc.FirstChildElement("Texture");
	if( pElement )
	{
		const char * strTexturePath = pElement->Attribute("path");
		if( ! strTexturePath )
			return E_FAIL;

		// texture
		m_pTexture = new Texture2D( strTexturePath );
		D_RETURN( m_pTexture->Create(false) );
	}
	else
	{
		return E_FAIL;
	}

	m_nRows = m_pTexture->GetHeight() / m_iIconHeight;
	m_nCols = m_pTexture->GetWidth() / m_iIconWidth;

	return S_OK;
}

HRESULT Atlas::Destroy()
{
	SAFE_DESTROY( m_pTexture );

	return S_OK;
}

bool Atlas::GetRectByIndex(unsigned int _Index, float & _t0x, float & _t0y, float & _t1x, float & _t1y) const
{
	if( _Index >= (unsigned int)( m_nCols * m_nRows ) )
		return false;

	float x = (float)( (_Index % m_nCols) * m_iIconWidth ) / (float)m_pTexture->GetWidth();
	float y = (float)( (_Index % m_nRows) * m_iIconHeight ) / (float)m_pTexture->GetHeight();

	_t0x = x;
	_t0y = y;

	_t1x = x + (float)m_iIconWidth / (float)m_pTexture->GetWidth();
	_t1y = y + (float)m_iIconHeight / (float)m_pTexture->GetHeight();

	return true;

}