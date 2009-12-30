#include "SceneTestShadow.h"

#define NB_STAGE 2


CSceneTestShadow::CSceneTestShadow(CMoteur* pMoteur):CScene(pMoteur)
{
	m_QualiterFiltrage = 1;
	m_pInterface = NULL;
}

CSceneTestShadow::~CSceneTestShadow()
{
	Console << "Deconstructeur CSceneTemplate" << endl;
}

// On vérifier que la carte graphique dispose des fonctions nécéssaire au rendu
bool CSceneTestShadow::ConfirmeFonction3D(D3DCAPS9* pCaps)
{
	if(!(pCaps->TextureCaps & D3DPTEXTURECAPS_PROJECTED))
	{
		Console << "Le peripherique ne permet pas la projection de texture" << endl;
		return false;
	}
	if(pCaps->MaxTextureWidth < 1024 || pCaps->MaxTextureHeight < 1024)
	{
		Console << "Le peripherique ne permet pas l'utilisation de texture de taille egale a 1024" << endl;
		return false;
	}
	if(!(pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP))
	{
		Console << "Le peripherique ne permet pas l'utilisation du bump mapping" << endl;
		return false;
	}
	if(pCaps->MaxSimultaneousTextures < 2)
	{
		Console << "Le peripherique ne permet pas l'utilisation de 2 etages de texture" << endl;
		return false;
	}
	if(pCaps->MaxUserClipPlanes < 1)
	{
		Console << "Le peripherique ne permet pas l'utilisation de plan de clipping" << endl;
		return false;
	}
	return true;
}

// Initialisation de la scène 3D
bool CSceneTestShadow::Initialisation()
{

	SetCamera(m_pCameraParDefaut);
	m_pCameraParDefaut->SetPosition(&D3DXVECTOR3(100.0f, 10.0f, 0.0f));

	// de Base la lumiere est activé
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pInterface = new CInterface(this);


	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );

	return true;


}

// Création des objets 3D de la scène
bool CSceneTestShadow::CreationObjet()
{
	// Type de filtrage des textures
	SetFiltreTexture(m_QualiterFiltrage);


	// Initialisation de la lumière 1 et activation

	// Calcule des matrices de la caméra
	m_pCameraCourante->SetMatrice();

	if(!m_pInterface->Creation())
		return false;


	//
	// Load up our teapot from a .x file...
	//

	LPD3DXBUFFER pD3DXMtrlBuffer;

	D3DXLoadMeshFromX( "teapot.x", D3DXMESH_SYSTEMMEM, m_pD3DDevice, NULL, 
		&pD3DXMtrlBuffer, NULL, &g_dwTeapotNumMtrls, &g_pTeapotMesh );

	// We need to extract the material properties and texture names 
	// from the pD3DXMtrlBuffer
	D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	g_pTeapotMtrls = new D3DMATERIAL9[g_dwTeapotNumMtrls];
	g_pTeapotTextures = new LPDIRECT3DTEXTURE9[g_dwTeapotNumMtrls];

	for( unsigned long i = 0; i < g_dwTeapotNumMtrls; ++i )
	{
		// Copy the material over...
		g_pTeapotMtrls[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		g_pTeapotMtrls[i].Ambient = g_pTeapotMtrls[i].Diffuse;

		// Create the texture...
		g_pTeapotTextures[i] = NULL;
		D3DXCreateTextureFromFile( m_pD3DDevice, d3dxMaterials[i].pTextureFilename, 
			&g_pTeapotTextures[i] );
	}

	pD3DXMtrlBuffer->Release();

	//
	// Cereate a single floor tile and its texture...
	//

	m_pD3DDevice->CreateVertexBuffer( 4*sizeof(FloorVertex),0, FloorVertex::FVF_Flags,
		D3DPOOL_DEFAULT, &g_pFloorVB, NULL );
	void *pVertices = NULL;

	g_pFloorVB->Lock( 0, sizeof(g_quadVertices), (void**)&pVertices, 0 );
	memcpy( pVertices, g_quadVertices, sizeof(g_quadVertices) );
	g_pFloorVB->Unlock();

	D3DXCreateTextureFromFile( m_pD3DDevice, "floor_tile.bmp", &g_pFloorTex );

	// Set up a material for the floor quads
	ZeroMemory( &g_pFloorMtrl, sizeof(D3DMATERIAL9) );
	g_pFloorMtrl.Diffuse.r = 1.0f;
	g_pFloorMtrl.Diffuse.g = 1.0f;
	g_pFloorMtrl.Diffuse.b = 1.0f;
	g_pFloorMtrl.Diffuse.a = 1.0f;
	g_pFloorMtrl.Ambient.r = 1.0f;
	g_pFloorMtrl.Ambient.g = 1.0f;
	g_pFloorMtrl.Ambient.b = 1.0f;
	g_pFloorMtrl.Ambient.a = 1.0f;

	//
	// Set up a point light source...
	//

	ZeroMemory( &g_light0, sizeof(D3DLIGHT9) );
	g_light0.Type         = D3DLIGHT_POINT;
	g_light0.Position     = D3DXVECTOR3( 2.0f, 2.0f, 0.0f ); // World-space position
	g_light0.Diffuse.r    = 1.0f;
	g_light0.Diffuse.g    = 1.0f;
	g_light0.Diffuse.b    = 1.0f;
	g_light0.Range        = 100.0f;
	g_light0.Attenuation0 = 1.0f;
	g_light0.Attenuation1 = 0.0f;

	m_pD3DDevice->SetLight( 0, &g_light0 );
	m_pD3DDevice->LightEnable( 0, TRUE );

	//
	// Create a big square for rendering the stencil buffer contents...
	//

	m_pD3DDevice->CreateVertexBuffer( 4*sizeof(ShadowVertex2),
		D3DUSAGE_WRITEONLY, ShadowVertex2::FVF_Flags,
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
	ShadowVertex2 *v;

	m_pBigSquareVB->Lock( 0, 0, (void**)&v, 0 );
	{
		v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
		v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
		v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
		v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
		D3DCOLOR color = 0x7fff0000;
		v[0].color = color;
		v[1].color = color;
		v[2].color = color;
		v[3].color = color;
	}
	m_pBigSquareVB->Unlock();

	//
	// Finally, construct a shadow volume object...
	//

	m_pShadowVolume = new ShadowVolume();

	return true;
}

// Destruction des resource utilisé par les objets
void CSceneTestShadow::DestructionObjet()
{

	if( g_pTeapotMtrls != NULL )
		delete[] g_pTeapotMtrls;

	if( g_pTeapotTextures != NULL )
	{
		for( unsigned long i = 0; i < g_dwTeapotNumMtrls; ++i )
		{
			if( g_pTeapotTextures[i] != NULL )
				g_pTeapotTextures[i]->Release();
		}

		delete[] g_pTeapotTextures;
	}

	if( g_pFloorVB != NULL )
		g_pFloorVB->Release(); 

	if( g_pTeapotMesh != NULL )
		g_pTeapotMesh->Release();

	m_pInterface->Release();
}

// Destruction des objets
void CSceneTestShadow::Destruction()
{

}

// Rendu 3D de la scène
void CSceneTestShadow::Rendu3D()
{
	//
	// Rendering the teapot mesh...
	//

	m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matTeapot );

	for( unsigned long n = 0; n < g_dwTeapotNumMtrls; ++n )
	{
		m_pD3DDevice->SetMaterial( &g_pTeapotMtrls[n] );
		m_pD3DDevice->SetTexture( 0, g_pTeapotTextures[n] );
		g_pTeapotMesh->DrawSubset( n );
	}

	//
	// Draw the floor by rendering a 25 x 25 grouping of textured quads.
	//

	m_pD3DDevice->SetTexture( 0, g_pFloorTex );
	m_pD3DDevice->SetMaterial( &g_pFloorMtrl );
	m_pD3DDevice->SetStreamSource( 0, g_pFloorVB, 0, sizeof(FloorVertex) );
	m_pD3DDevice->SetFVF( FloorVertex::FVF_Flags );

	D3DXMATRIX matFloor;
	float x = 0.0f;
	float z = 0.0f;

	for( int i = 0; i < 25; ++i )
	{
		for( int j = 0; j < 25; ++j )
		{
			D3DXMatrixTranslation( &matFloor, x - 25.0f, -3.0f, z - 25.0f );
			m_pD3DDevice->SetTransform( D3DTS_WORLD, &matFloor );
			m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

			x += 2.0f;
		}
		x  =  0.0f;
		z += 2.0f;
	}

	//
	// If the user selected this option, render the shadow volume geometry...
	//

	if( g_bMakeShadowVolumeVisible == true )
	{
		// Render shadow volume in wire-frame mode
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matTeapot );
		m_pShadowVolume->render( m_pD3DDevice );
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	}

	//
	// Render the shadow volume into the stencil buffer, then render it 
	// to the scene.
	//

	renderShadowToStencilBuffer();
	renderShadowToScene();
	m_pInterface->afficher();
}

// Calculs des animations de la scène
void CSceneTestShadow::Animation(double TempsEcouler, double DeltaTemps)
{
	// On remet la caméra dans un état normal
	GetCameraCourante()->SetMatrice();
}

// Test des entrées des périphériques
void CSceneTestShadow::Aquisition(BYTE EtatClavier[], DIMOUSESTATE* pEtatSouris,DIJOYSTATE2* pEtatJoystick)
{
	// On test si on a changé un paramètre de qualité
	if(KEYDOWN(DIK_F1))
	{
		m_QualiterFiltrage = 0;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F2))
	{
		m_QualiterFiltrage = 1;
		SetFiltreTexture(m_QualiterFiltrage);
	}
	if(KEYDOWN(DIK_F3))
	{
		m_QualiterFiltrage = 2;
		SetFiltreTexture(m_QualiterFiltrage);
	}
}

// Appliqué les paramètres de filtrage de texture
void CSceneTestShadow::SetFiltreTexture(int Type)
{
	switch(Type)
	{
		// Filtrage Bilinéaire
	case 0:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		}

		break;

		// Filtrage Tilinéaire
	case 1:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}
		break;

		// Filtrage Anisotropic
	case 2:
		for (int i = 0; i<NB_STAGE ; i++)
		{
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 16);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pD3DDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
		}
		break;
	}
}


//-----------------------------------------------------------------------------
// Name: renderShadowToStencilBuffer()
// Desc:
//-----------------------------------------------------------------------------
void CSceneTestShadow::renderShadowToStencilBuffer( void )
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

	if( g_bTwoSidedStencilsAvailable == true )
	{
		// With 2-sided stencil, we can avoid rendering twice:
		m_pD3DDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, TRUE );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILFUNC,  D3DCMP_ALWAYS );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILFAIL,  D3DSTENCILOP_KEEP );
		m_pD3DDevice->SetRenderState( D3DRS_CCW_STENCILPASS, D3DSTENCILOP_DECR );

		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_NONE );

		// Draw both sides of shadow volume in stencil/z only
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matTeapot );
		m_pShadowVolume->render( m_pD3DDevice );

		m_pD3DDevice->SetRenderState( D3DRS_TWOSIDEDSTENCILMODE, FALSE );
	}
	else
	{
		// Draw front-side of shadow volume in stencil/z only
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matTeapot );
		m_pShadowVolume->render( m_pD3DDevice );

		// Now reverse cull order so back sides of shadow volume are written.
		m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

		// Decrement stencil buffer value
		m_pD3DDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_DECR );

		// Draw back-side of shadow volume in stencil/z only
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_matTeapot );
		m_pShadowVolume->render( m_pD3DDevice );
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
void CSceneTestShadow::renderShadowToScene( void )
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
	m_pD3DDevice->SetFVF( ShadowVertex2::FVF_Flags );
	m_pD3DDevice->SetStreamSource( 0, m_pBigSquareVB, 0, sizeof(ShadowVertex2) );
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	// Restore render states
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}