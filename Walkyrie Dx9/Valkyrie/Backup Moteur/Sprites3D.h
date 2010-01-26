#ifndef CSPRITE_H
#define CSPRITE_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "DXPileEtats.h"
/*
* Cette classe permet la gestion des sprites3D.
*/

class CSprite3D
{
	struct SPRITE3D_VERTEX
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Normal;
		D3DCOLOR    Color;
		FLOAT       Tu;
		FLOAT       Tv;

		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL |D3DFVF_DIFFUSE | D3DFVF_TEX1;
	};

protected:

	LPDIRECT3DDEVICE9		m_pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DTEXTURE9      m_pTexture;

	CDXPileEtats*		DXEtats;

	LPSTR m_pFileTexture;



	float               m_fRX;
	float               m_fRY;
	float               m_fRZ;

	float				m_fTX;
	float				m_fTY;
	float				m_fTZ;

	bool                m_bIsAlphaBlended;

	bool					m_bExternalTexture;
	float                   m_fWidth;
	float                   m_fHeight;

public:

	CSprite3D(LPDIRECT3DDEVICE9 pD3DDevice= NULL);
	~CSprite3D();

public:

	float GetWidth       ();
	void  SetWidth       ( float Value );

	float GetHeight      ();
	void  SetHeight      ( float Value );

	float GetRotationX   ();
	void  SetRotationX   ( float Value );

	float GetRotationY   ();
	void  SetRotationY   ( float Value );

	float GetRotationZ   ();
	void  SetRotationZ   ( float Value );

	void SetRotationXYZ( float fXValue ,float fYValue , float fZValue);

	float GetTranslationX();
	void  SetTranslationX( float Value );

	float GetTranslationY();
	void  SetTranslationY( float Value );

	float GetTranslationZ();
	void  SetTranslationZ( float Value );

	void  SetTranslationXYZ( float fXValue ,float fYValue , float fZValue);

	virtual D3DXMATRIXA16 GetWorldMatrix();

	void SetTexture( LPDIRECT3DTEXTURE9 Texture );

	virtual void RenderSprite3D(bool SaveState = true);
	virtual bool  ChargerSprites3D(LPSTR pFileTexture, float fWidth = 1.0f, float fHeight = 1.0f, float fRepeatTextureX = 1.0f ,float fRepeatTextureY = 1.0f);
	virtual void Release();
	bool RestoreSprite3D();

private:
	void SetAlphaBlending    ();
};

#endif // E_SPRITE_H
