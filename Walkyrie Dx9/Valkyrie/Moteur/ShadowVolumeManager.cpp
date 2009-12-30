#include "ShadowVolumeManager.h"


CShadowVolumeManager :: CShadowVolumeManager(CScene* pScene)
{

	m_pD3DDevice = pScene->Get3DDevice();
	m_pScene = pScene;
	m_nIndiceShadowVolumes = 0;
}


CShadowVolumeManager::~CShadowVolumeManager()
{

}

void CShadowVolumeManager:: InitShadowVolumeManager(D3DCOLOR color)
{
	//
	// Create a big square for rendering the stencil buffer contents...
	//

	m_pD3DDevice->CreateVertexBuffer( 4*sizeof(ShadowVertex),
										D3DUSAGE_WRITEONLY, ShadowVertex::FVF_Flags,
										D3DPOOL_MANAGED, &m_pBigSquareVB, NULL );

	// Get the width & height of the back-buffer.
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	D3DSURFACE_DESC d3dsd;
	m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
										pBackBuffer->GetDesc( &d3dsd );
										pBackBuffer->Release();
	float sx = (float)d3dsd.Width;
	float sy = (float)d3dsd.Height;

	// Set the size of the big square shadow
	ShadowVertex *v;

	m_pBigSquareVB->Lock( 0, 0, (void**)&v, 0 );
	{
		v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
		v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
		v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
		v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
		v[0].color = color;
		v[1].color = color;
		v[2].color = color;
		v[3].color = color;
	}

	m_pBigSquareVB->Unlock();
}

void CShadowVolumeManager :: InitLight(D3DXVECTOR3 vLight )
{
		m_pvlight = vLight;
}

//-----------------------------------------------------------------------------
// Name: renderShadowToStencilBuffer()
// Desc:
//-----------------------------------------------------------------------------
void CShadowVolumeManager :: renderShadowToStencilBuffer( void )
{
	// Disable z-buffer writes (note: z-testing still occurs), and enable the
	// stencil-buffer
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE, TRUE );

	// Dont bother with interpolating color
	m_pD3DDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );

	// Set up stencil compare fuction, reference value, and masks.
	// Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
	// Note: since we set up the stencil-test to always pass, the STENCILFAIL
	// renderstate is really not needed.
	m_pD3DDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

	// If z-test passes, inc/decrement stencil buffer value
	m_pD3DDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILPASS,      D3DSTENCILOP_INCR );

	// Make sure that no pixels get drawn to the frame buffer
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	if( m_bTwoSidedStencilsAvailable == true )
	{
		// With 2-sided stencil, we can avoid rendering twice:
		m_pD3DDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILFUNC,  D3DCMP_ALWAYS );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILFAIL,  D3DSTENCILOP_KEEP );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR );

		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE );

		for(int i = 0; i < m_nIndiceShadowVolumes; i++)
		{
			m_pShadowVolume[i].render( m_pD3DDevice );
		}

		m_pD3DDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, FALSE );
	}
	else
	{
		for(int i = 0; i < m_nIndiceShadowVolumes; i++)
		{
			m_pShadowVolume[i].render( m_pD3DDevice );
		}

		// Now reverse cull order so back sides of shadow volume are written.
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

		// Decrement stencil buffer value
		m_pD3DDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

		for(int i = 0; i < m_nIndiceShadowVolumes; i++)
		{
			m_pShadowVolume[i].render( m_pD3DDevice );
		}

	}

	// Restore render states
	m_pD3DDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

//-----------------------------------------------------------------------------
// Name: renderShadowToScene()
// Desc: Draws a big gray polygon over scene according to the mask in the
//       stencil buffer. (Any pixel with stencil==1 is in the shadow.)
//-----------------------------------------------------------------------------
void CShadowVolumeManager :: renderShadowToScene( void )
{
	// Set render states
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );



	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

	// Only write where stencil value >= 1 (count indicates # of shadows that
	// overlap that pixel)
	m_pD3DDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );


	// Draw a big, gray square
	m_pD3DDevice->SetFVF( ShadowVertex::FVF_Flags );
	m_pD3DDevice->SetStreamSource( 0, m_pBigSquareVB, 0, sizeof(ShadowVertex) );
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	// Restore render states
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}



void CShadowVolumeManager :: clearShadowVolumes()
{
	
	for(int i = 0; i<m_nIndiceShadowVolumes;  i++)
		m_pShadowVolume[i].clear();

	m_nIndiceShadowVolumes = 0;
	
}

HRESULT CShadowVolumeManager :: AddAndBuildShadowVolumes( CMesh* pObject)
{
	
//	assert(m_nIndiceShadowVolumes < MAX_VOLUME);

	HRESULT hr = m_pShadowVolume[m_nIndiceShadowVolumes].buildShadowVolume(pObject, m_pvlight);
	if(!FAILED(hr))
	{
		m_nIndiceShadowVolumes++;
	}
	return hr;

}

void CShadowVolumeManager :: RenduShadow()
{
	renderShadowToStencilBuffer();
	renderShadowToScene();
}

void CShadowVolumeManager :: RenduShadowVolume()
{
	for(int i = 0; i < m_nIndiceShadowVolumes; i++)
	{
		m_pShadowVolume[i].render( m_pD3DDevice );
	}
}


void CShadowVolumeManager :: Release()
{
	SAFE_RELEASE(m_pBigSquareVB);
}

void CShadowVolumeManager :: ReBuildShadowVolume()
{
	for(int i = 0; i < m_nIndiceShadowVolumes; i++)
	{

		m_pShadowVolume[i].ReBuildShadowVolume(m_pvlight);
	}
}


void CShadowVolumeManager :: Reset()
{
	for(int i = 0; i < m_nIndiceShadowVolumes; i++)
	{
		m_pShadowVolume[i].Reset();
	}
}

