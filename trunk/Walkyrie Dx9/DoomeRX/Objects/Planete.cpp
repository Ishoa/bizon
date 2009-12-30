#include "Planete.h"


///////////////////////////////////////////////////////////////////////////////
// Creation d'une bumpmap a partir d'une carte d'elevation

bool CPlanete::CreateBumpmapFromFile( char *fname, LPDIRECT3DTEXTURE9 *pBMP ) {
	HRESULT		s;
	UINT		dSz, sSz;

	// lecture de la carte d'elevation
	LPDIRECT3DTEXTURE9      pTXa = NULL;  // texture contenant la carte d'elevation
	D3DSURFACE_DESC			sd;
	s = D3DXCreateTextureFromFile( m_pD3DDevice, fname, &pTXa);
	if FAILED(s) { 
		Console<<fname<<"introuvable."<<endl;
		return false;
	}

	// pas entre 2 valeurs en fonction du format de la texture
	pTXa->GetLevelDesc( 0, &sd );
	switch(sd.Format) {
	case D3DFMT_R8G8B8:		sSz=3; break;
	case D3DFMT_A8R8G8B8: 
	case D3DFMT_X8R8G8B8:	sSz=4; break;
	case D3DFMT_L8:			sSz=1; break;
	default:
		Console<<"Format "<<sd.Format<<" non implemente\nMettre swith a jour ligne"<<endl;
		return false;
	}

	// recuperation de la description de la texture
	LPDIRECT3DTEXTURE9      pBMa = NULL;  // texture (renvoyee) contenant les differences
	s = m_pD3DDevice->CreateTexture( sd.Width, sd.Height, 1, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, &pBMa, NULL );
	dSz = 2;
	if FAILED(s) {
		Console<<"Format V8U8 non supporté."<<endl;
		return false;
	}

	//	pDst = pSrc = 0;  
	UINT                u,v,pSrc,pDst,dPitch,sPitch;
	BYTE               *pU;
	unsigned char      *pH;
	D3DLOCKED_RECT   rect;
	// verouillage de la highmap et de la bumpmap
	pTXa->LockRect(0,&rect,NULL,0);
	sPitch = rect.Pitch;
	pSrc = sPitch - sd.Width*sSz; // dSz nombre de bytes par pixel dans l'image originale
	pH = (unsigned char *)rect.pBits;
	if (sSz==4) pH++;

	pBMa->LockRect(0,&rect,NULL,0);
	dPitch = rect.Pitch;
	pDst = dPitch - sd.Width*dSz; // 2 = nombre de bytes par pixel dans une surface V8U8
	pU = (BYTE *)rect.pBits;

	// if (dSz==4) pH++; // pour ne pas lire le alpha
	// construction de la bumpmap
	memset(pU,0,rect.Pitch*sd.Height);
	for(u=0;u<sd.Height;u++) { 
		for(v=0;v<sd.Width;v++) { 
			pU[ u*dPitch + dSz*v  ] = (BYTE)( (- long(pH[ u*sPitch + (v+sSz)%sd.Width  ]) + long(pH[ u*sPitch + (v-sSz)%sd.Width  ]) ) );//>> 1 );
			pU[ u*dPitch + dSz*v+1] = (BYTE)( (- long(pH[ ((u+1)%sd.Height)*sPitch + v ]) + long(pH[ ((u-1)%sd.Height)*sPitch + v ]) ) );//>> 1 );
		}
	}
	// deverouillage
	pBMa->UnlockRect(0);
	pTXa->UnlockRect(0);
	SAFE_RELEASE(pTXa);

	*pBMP = pBMa;
	return true;
}


//############################################### CPLANETE ###########################

CPlanete::CPlanete()
{



}

CPlanete::CPlanete(CScene* pScene)
{
	m_pVertexBuffer  = NULL;
	m_pIndexBuffer = NULL;   // index buffer
	m_pTexture = NULL;  // texture
	m_pBumpMap = NULL;  // bumpmap

	m_nVERTEXsz = sizeof(VERTEX_PLANETE);

	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_bBumpMappingMode = true;

	m_fbscale = 1.f;
	m_fdscale = 1.f;

}

CPlanete::~CPlanete()
{

}


bool CPlanete::Creation(MTYPEPLANETE eTypePlanete,float nTaille,int nDiv, char* NomFichierTexture,char* NomFichierBumpMap)
{
	m_eTypePlanete = eTypePlanete;
	m_fTaille = nTaille;
	m_ndiv=nDiv;
	m_nvert = (m_ndiv+1)*(2*m_ndiv+1);
	m_nfac = ((m_ndiv-2)*(2*m_ndiv)*2 + 2*(2*m_ndiv))*3;


	// definition d'une sphere
	VERTEX_PLANETE*  sph = new VERTEX_PLANETE[m_nvert];
	//VERTEX_PLANETE  sph[nvert];
	{
		float   ph=-D3DX_PI/2, th, da = D3DX_PI/m_ndiv;
		int	    i,j,k=0;
		for(i=0; i<=m_ndiv;i++,ph+=da) {
			for(j=0,th=0;j<=2*m_ndiv;j++,th+=da,k++) {
				sph[k].P.x = cosf(th)*cosf(ph);
				sph[k].P.y = sinf(th)*cosf(ph);
				sph[k].P.z = sinf(ph); 
				sph[k].N = sph[k].P;
				sph[k].P.x *= nTaille;
				sph[k].P.y *= nTaille;
				sph[k].P.z *= nTaille;
				sph[k].cd = D3DCOLOR_XRGB(255,255,255);
				sph[k].u1 = i/(float)m_ndiv;
				sph[k].v1 = j/(float)(2*m_ndiv);
			}
		}
	}
		
	// creation du vertex buffer
    void *ptr = NULL;
	m_pD3DDevice->CreateVertexBuffer( m_nvert*sizeof(VERTEX_PLANETE),0, D3DFVF_VERTEX_PLANETE,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );
    m_pVertexBuffer->Lock( 0, m_nvert*sizeof(VERTEX_PLANETE), (void**)&ptr, 0 );
    memcpy(ptr, sph, m_nvert*sizeof(VERTEX_PLANETE) );
    m_pVertexBuffer->Unlock();

	// creation de l'index buffer
	shortindex* ind = new shortindex [m_nfac];
	//shortindex ind[nfac];
	{	int i,j,k=0,d=0,n=2*m_ndiv;
		for(j=0; j<n; j++) { ind[k++]=d+j; ind[k++]=d+n+1+j+1; ind[k++]=d+n+1+j; }
		d+=n+1;
		for(i=0;i<m_ndiv-2;i++,d+=n+1) for(j=0; j<n; j++) {
				ind[k++]=d+j; ind[k++]=d+n+1+j+1; ind[k++]=d+n+1+j; 
				ind[k++]=d+j; ind[k++]=d+j+1;     ind[k++]=d+n+1+j+1; 
		}
		for(j=0; j<n; j++) { ind[k++]=d+j; ind[k++]=d+j+1; ind[k++]=d+n+1+j+1; }
	}

	m_pD3DDevice->CreateIndexBuffer(m_nfac*sizeof(shortindex),0,D3DFMT_INDEX16, D3DPOOL_MANAGED,&m_pIndexBuffer,NULL);
	m_pIndexBuffer->Lock(0,m_nfac*sizeof(shortindex),&ptr,0);
	memcpy(ptr,ind,m_nfac*sizeof(shortindex));
	m_pIndexBuffer->Unlock();

	HRESULT s = D3DXCreateTextureFromFile( m_pD3DDevice, NomFichierTexture, &m_pTexture);
	if FAILED(s) {
		Console<<"texture planete introuvable"<<endl;;
		return false;
    }


	if (!CreateBumpmapFromFile( NomFichierBumpMap, &m_pBumpMap))
	{
		Console<<"carte d'élévation introuvable"<<endl;
		return false;
    }

	return true;	

}

void CPlanete::augmenterdScale(float fscale)
{
	m_fdscale += fscale;

}

void CPlanete::augmenterbScale(float fscale)
{
	m_fbscale += fscale;
}

void CPlanete::diminuerdScale(float fscale)
{
	m_fdscale -= fscale;

}

void CPlanete::diminuerbScale(float fscale)
{
	m_fbscale -= fscale;
}


void CPlanete::RenduModel()
{
	
	// chargement des textures et initialisation des etages
	// etage 0 : inversion des axes x et y de la texture
	D3DMATRIX Minvaxe={0,1,0,0 , 1,0,0,0 , 0,0,1,0 , 0,0,0,1};
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE0,&Minvaxe);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);

	// etage 1 : inversion des axes x et y de la texture
	//           + utilisation de l'ensemble de coordonnees No 0
	m_pD3DDevice->SetTransform(D3DTS_TEXTURE1,&Minvaxe);
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_TEXCOORDINDEX,0);

	m_pD3DDevice->SetTransform(D3DTS_TEXTURE2,&Minvaxe);
	m_pD3DDevice->SetTextureStageState(2,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	m_pD3DDevice->SetTextureStageState(2,D3DTSS_TEXCOORDINDEX,0);

	// texture utilisee
	int   s=-1;

	if (m_bBumpMappingMode==1) {
		s++;
		m_pD3DDevice->SetTexture(s,m_pBumpMap);
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAP ); // D3DTOP_BUMPENVMAPLUMINANCE ); // 
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLORARG2, D3DTA_CURRENT );
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVMAT00, F2DW(m_fdscale));
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVMAT01, F2DW(0.f));
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVMAT10, F2DW(0.f));
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVMAT11, F2DW(m_fdscale));
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVLSCALE, F2DW(m_fbscale));
		m_pD3DDevice->SetTextureStageState( s, D3DTSS_BUMPENVLOFFSET, F2DW(0.f));
	}

	s++;
	m_pD3DDevice->SetTexture(s,m_pTexture);
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLORARG2, D3DTA_CURRENT ); //D3DTA_DIFFUSE 
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	s++;
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	m_pD3DDevice->SetTextureStageState( s, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	
	// dessin de la sphere

    m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, m_nVERTEXsz );
    m_pD3DDevice->SetFVF( D3DFVF_VERTEX_PLANETE );
	m_pD3DDevice->SetIndices(m_pIndexBuffer);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_nvert,0,m_nfac/3);
 

	for( int i = 0; i<3;i++)
	{
		m_pD3DDevice->SetTextureStageState( i, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
		m_pD3DDevice->SetTextureStageState( i, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		m_pD3DDevice->SetTextureStageState( i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
}


void CPlanete::Release()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pBumpMap);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
}
	
char* CPlanete::getDescription(int* taille)
{



	switch(m_eTypePlanete)
	{
	case SOLEIL:
		{
			char resTmp [] = "SOLEIL\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case TERRE:
		{
			char resTmp [] = "TERRE\n\nDiametre: 40 075 km\nSurface: 510 067 420 km2\nGravité: 9,780 m\∙s-2\nPopulation: 6,705 milliards";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	
	case MARS: 
		{
			char resTmp [] = "MARS\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}

	case MERCURE:
		{
			char resTmp [] = "MERCURE\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case VENUS: 
		{
			char resTmp [] = "VENUS\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case JUPITER:
		{
			char resTmp [] = "JUPITER\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case SATURN:
		{
			char resTmp [] = "SATURN\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case URANUS:
		{
			char resTmp [] = "URANUS\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case NEPTUNE:
		{
			char resTmp [] = "NEPTUNE\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case PLUTON: 
		{
			char resTmp [] = "PLUTON\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	case LUNE:
		{
			char resTmp [] = "LUNE\n\nDiametre: 1 392 000 km\nSurface: 6,09×10^12 km2\nGravité: 273,95 m\∙s-2\nPopulation: 0";
			taille[0] = sizeof(resTmp);
			char* res= new char[taille[0]];
			for(int i=0;i<taille[0];i++)
				res[i]=resTmp[i];
			return res;
		}
	default : 
		{
			taille[0]=0;
			return NULL;
		}

	}
	

	
}