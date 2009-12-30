#include "BumpMappingObjet.h"


CBumpMappingObjet::CBumpMappingObjet()
{


}


CBumpMappingObjet::CBumpMappingObjet(CScene* pScene)
{
	m_pVertexBuffer = NULL;

	m_pTangents = NULL;
	m_pBiNormals = NULL;
	m_pNormals = NULL;


	m_pScene = pScene;
	m_pD3DDevice = m_pScene->Get3DDevice();

	m_nNbVertex = 0;
	m_bDoDot3BumpMapping = true;
	m_bTexture = true;

}


CBumpMappingObjet::~CBumpMappingObjet()
{

}

bool CBumpMappingObjet::CreationObjectVertexBuffer(Vertex* pVertexBuffer, int nNbVertex, char* NomFichierTexture,char* NomFichierNormalMap)
{
	m_nNbVertex = nNbVertex;



	
	if FAILED(!m_pNormalMapTexture.chargerTexture( m_pD3DDevice, NomFichierNormalMap )) 
	{
		Console<<"Probleme chargement texture : "<<NomFichierNormalMap<<endl;
		return false;
	}

	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	
	if FAILED(!m_pTexture.chargerTexture( m_pD3DDevice, NomFichierTexture)) {
		Console<<"Probleme chargement normal texture : "<<NomFichierTexture<<endl;
		return false;
	}

	m_pD3DDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pD3DDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


	m_pD3DDevice->CreateVertexBuffer( nNbVertex*sizeof(Vertex), D3DUSAGE_DYNAMIC, 
										Vertex::FVF_Flags, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );
	void *pVertices = NULL;

	m_pVertexBuffer->Lock( 0, nNbVertex*sizeof(Vertex), (void**)&pVertices, 0 );
	memcpy(pVertices, pVertexBuffer, nNbVertex*sizeof(Vertex) );
	m_pVertexBuffer->Unlock();


	m_pTangents = new D3DXVECTOR3[nNbVertex];
	m_pBiNormals = new D3DXVECTOR3[nNbVertex];
	m_pNormals = new D3DXVECTOR3[nNbVertex];


	for( int i = 0; i < nNbVertex; ++i )
	{
		m_pTangents[i]  = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		m_pBiNormals[i] = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		m_pNormals[i]   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	}

	computeTangentsMatricesForEachVertex();

	return true;
}


bool CBumpMappingObjet::CreationObjectVertexBufferI(Vertex* pVertexBuffer, int nNbVertex, char* NomFichierTexture,char* NomFichierNormalMap)
{

	return true;
}


void CBumpMappingObjet::Release()
{

	m_pTexture.Release();
	m_pNormalMapTexture.Release();
	SAFE_RELEASE(m_pVertexBuffer);

	SAFE_DELETE_ARRAY(m_pTangents);
	SAFE_DELETE_ARRAY(m_pBiNormals);
	SAFE_DELETE_ARRAY(m_pNormals);
}

void CBumpMappingObjet::RenduModel(CLumiere lumiere, D3DXMATRIX* matWorld)
{

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	if( m_bDoDot3BumpMapping == true )
	{
		//
		// Render a Dot3 bump mapped cube...
		//

		//
		// Transform the light's position from world-space to model-space
		//

		D3DXVECTOR3 vLightPosWS;    // Light position (in world-space)
		D3DXVECTOR3 vLightPosMS;    // Light position (in model-space)
		D3DXVECTOR3 vVertToLightMS; // L vector of N.L (in model-space)
		D3DXVECTOR3 vVertToLightTS; // L vector of N.L (in tangent-space)

		// Get the light's current position, which is in world-space.
		vLightPosWS = (lumiere.GetLumiere())->Position;

		// Transform the light's position into model-space
		D3DXMATRIX worldInverse;
		D3DXMatrixInverse( &worldInverse, NULL, matWorld );
		D3DXVec3TransformCoord( &vLightPosMS, &vLightPosWS, &worldInverse );

		//
		// Since our cube's vertex data is stored in a Vertex Buffer, we will
		// need to lock it briefly so we can encode the new tangent-space
		// L vectors that we're going to create into the diffuse color of each 
		// vertex.
		//

		Vertex *pVertices = NULL;
		m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );

		for( int i = 0; i < m_nNbVertex; ++i )
		{
			//
			// For each vertex, rotate L (of N.L) into tangent-space and 
			// pass it into Direct3D's texture blending system by packing it 
			// into the vertex's diffuse color.
			//

			D3DXVECTOR3 vCurrentVertex = D3DXVECTOR3( pVertices[i].x, 
				pVertices[i].y, 
				pVertices[i].z );

			vVertToLightMS = vLightPosMS - vCurrentVertex;
			D3DXVec3Normalize( &vVertToLightMS, &vVertToLightMS );

			//
			// Build up an inverse tangent-space matrix using the Tangent, 
			// Binormal, and Normal calculated for the current vertex and 
			// then use it to transform our L vector (of N.L), which is in 
			// model-space, into tangent-space.
			//
			// A tangent matrix is of the form:
			//
			// |Tx Bx Nx 0|
			// |Ty By Ny 0|
			// |Tz Bz Nz 0|
			// |0  0  0  1|
			//
			// Note: Our vectors have already been inverted, so there is no 
			// need to invert our tangent matrix once we build it up.
			//

			//                           Tangent           Binormal           Normal
			D3DXMATRIX invTangentMatrix( m_pTangents[i].x, m_pBiNormals[i].x, m_pNormals[i].x, 0.0f,
				m_pTangents[i].y, m_pBiNormals[i].y, m_pNormals[i].y, 0.0f,
				m_pTangents[i].z, m_pBiNormals[i].z, m_pNormals[i].z, 0.0f,
				0.0f,             0.0f,              0.0f,            1.0f );

			D3DXVec3TransformNormal( &vVertToLightTS, &vVertToLightMS, &invTangentMatrix );

			//
			// Last but not least, we must encode our new L vector as a DWORD 
			// value so we can set it as the new vertex color. Of course, the 
			// hardware assumes that we are  going to do this, so it will 
			// simply decode the original vector back out by reversing the 
			// DOWRD encdoing we've performed here.
			//

			pVertices[i].diffuse = encodeVectorAsDWORDColor( &vVertToLightTS );
		}

		m_pVertexBuffer->Unlock();

		//
		// STAGE 0
		//
		// Use D3DTOP_DOTPRODUCT3 to find the dot-product of (N.L), where N is 
		// stored in the normal map, and L is passed in as the vertex color - 
		// D3DTA_DIFFUSE.
		//

		m_pD3DDevice->SetTexture( 0, m_pNormalMapTexture.getDXTexture() );
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );

		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3 ); // Perform a Dot3 operation...
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );    // between the N (of N.L) which is stored in a normal map texture...
		m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );    // with the L (of N.L) which is stored in the vertex's diffuse color.

		//
		// STAGE 1
		//
		// Modulate the base texture by N.L calculated in STAGE 0.
		//

		m_pD3DDevice->SetTexture( 1, m_pTexture.getDXTexture() );
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );

		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE ); // Modulate...
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE ); // the texture for this stage with...
		m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT ); // the current argument passed down from stage 0
	}
	else
	{
		//
		// Render a regular textured cube with no Dot3 bump mapping...
		//

		// Reset the vertex colors back to pure white...
		Vertex *pVertices = NULL;
		m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
		for( int i = 0; i < m_nNbVertex; ++i )
			pVertices[i].diffuse = D3DCOLOR_COLORVALUE(1.0f,1.0f,1.0f,1.0f);
		m_pVertexBuffer->Unlock();

		if(m_bTexture)
		{
			//
			// STAGE 0
			//

			m_pD3DDevice->SetTexture( 0, m_pTexture.getDXTexture() );

			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			//
			// STAGE 1
			//

			m_pD3DDevice->SetTexture( 1, NULL );

			m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
			m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		}
		else
		{

			m_pD3DDevice->SetTexture( 0, NULL );
			m_pD3DDevice->SetTexture( 1, NULL );
		}

	}


	//
	// Now, render our textured test cube, which consists of 6 cube faces...
	//

	m_pD3DDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(Vertex) );
	m_pD3DDevice->SetFVF( Vertex::FVF_Flags );

	//for(int i = 0 ; i<m_nNbVertex; i+=3)
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST,  0, m_nNbVertex/3);

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->SetTexture( 1, NULL );

}


//-----------------------------------------------------------------------------
// Name: computeTangentsMatricesForEachVertex()
// Desc: Espace tangent
//-----------------------------------------------------------------------------
void CBumpMappingObjet::computeTangentsMatricesForEachVertex()
{

	Vertex *pVertices = NULL;
	m_pVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );

	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;
	D3DXVECTOR3 v3;
	D3DXVECTOR3 vTangent;
	D3DXVECTOR3 vBiNormal;
	D3DXVECTOR3 vNormal;


	for( int i = 0; i < m_nNbVertex; i += 3 ) 
	{
		//
		// Vertex 0 of current Triangle face...
		//
		//  v3
		//    2 
		//    | \
		//    |  \  
		//    |   \
		//    0----1
		//  v1      v2
		//

		v1 = D3DXVECTOR3(pVertices[i].x,pVertices[i].y,pVertices[i].z);
		v2 = D3DXVECTOR3(pVertices[i+1].x,pVertices[i+1].y,pVertices[i+1].z);
		v3 = D3DXVECTOR3(pVertices[i+2].x,pVertices[i+2].y,pVertices[i+2].z);

		createTangentSpaceVectors( &v1,&v2,&v3,
			pVertices[i].tu1, pVertices[i].tv1,
			pVertices[i+1].tu1, pVertices[i+1].tv1,
			pVertices[i+2].tu1, pVertices[i+2].tv1,
			&vTangent, &vBiNormal, &vNormal );

		m_pTangents[i]  = vTangent;
		m_pBiNormals[i] = vBiNormal;
		m_pNormals[i]   = vNormal;

		//
		// Vertex 1 of current Triangle face...
		//
		//  v3
		//    2 
		//    | \
		//    |  \  
		//    |   \
		//    0----1
		//  v2      v1
		//

		v1 = D3DXVECTOR3(pVertices[i+1].x,pVertices[i+1].y,pVertices[i+1].z);
		v2 = D3DXVECTOR3(pVertices[i].x,pVertices[i].y,pVertices[i].z);
		v3 = D3DXVECTOR3(pVertices[i+2].x,pVertices[i+2].y,pVertices[i+2].z);

		createTangentSpaceVectors( &v1,&v2,&v3,
			pVertices[i+1].tu1, pVertices[i+1].tv1,
			pVertices[i].tu1, pVertices[i].tv1,
			pVertices[i+2].tu1, pVertices[i+2].tv1,
			&vTangent, &vBiNormal, &vNormal );

		m_pTangents[i+1]  = vTangent;
		m_pBiNormals[i+1] = vBiNormal;
		m_pNormals[i+1]   = vNormal;

		//
		// Vertex 2 of current Triangle face...
		//
		//  v1
		//    2 
		//    | \
		//    |  \  
		//    |   \
		//    0----1
		//  v2      v3
		//

		v1 = D3DXVECTOR3(pVertices[i+2].x,pVertices[i+2].y,pVertices[i+2].z);
		v2 = D3DXVECTOR3(pVertices[i].x,pVertices[i].y,pVertices[i].z);
		v3 = D3DXVECTOR3(pVertices[i+1].x,pVertices[i+1].y,pVertices[i+1].z);

		createTangentSpaceVectors( &v1,&v2,&v3,
			pVertices[i+2].tu1, pVertices[i+2].tv1,
			pVertices[i].tu1, pVertices[i].tv1,
			pVertices[i+1].tu1, pVertices[i+1].tv1,
			&vTangent, &vBiNormal, &vNormal );

		m_pTangents[i+2]  = vTangent;
		m_pBiNormals[i+2] = vBiNormal;
		m_pNormals[i+2]   = vNormal;

	}

	m_pVertexBuffer->Unlock();
}


void CBumpMappingObjet::createTangentSpaceVectors( D3DXVECTOR3 *v1,
							   D3DXVECTOR3 *v2,
							   D3DXVECTOR3 *v3,
							   float v1u, float v1v,
							   float v2u, float v2v,
							   float v3u, float v3v,
							   D3DXVECTOR3 *vTangent,
							   D3DXVECTOR3 *vBiNormal,
							   D3DXVECTOR3 *vNormal )
{
	// Create edge vectors from vertex 1 to vectors 2 and 3.
	D3DXVECTOR3 vDirVec_v2_to_v1 = *v2 - *v1;
	D3DXVECTOR3 vDirVec_v3_to_v1 = *v3 - *v1;

	// Create edge vectors from the texture coordinates of vertex 1 to vector 2.
	float vDirVec_v2u_to_v1u = v2u - v1u;
	float vDirVec_v2v_to_v1v = v2v - v1v;

	// Create edge vectors from the texture coordinates of vertex 1 to vector 3.
	float vDirVec_v3u_to_v1u = v3u - v1u;
	float vDirVec_v3v_to_v1v = v3v - v1v;

	float fDenominator = vDirVec_v2u_to_v1u * vDirVec_v3v_to_v1v - 
		vDirVec_v3u_to_v1u * vDirVec_v2v_to_v1v;

	if( fDenominator < 0.0001f && fDenominator > -0.0001f )
	{
		// We're too close to zero and we're at risk of a divide-by-zero! 
		// Set the tangent matrix to the identity matrix and do nothing.
		*vTangent  = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		*vBiNormal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		*vNormal   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	}
	else
	{
		// Calculate and cache the reciprocal value
		float fScale1 = 1.0f / fDenominator;

		D3DXVECTOR3 T;
		D3DXVECTOR3 B;
		D3DXVECTOR3 N;

		T = D3DXVECTOR3((vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.x - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.x) * fScale1,
			(vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.y - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.y) * fScale1,
			(vDirVec_v3v_to_v1v * vDirVec_v2_to_v1.z - vDirVec_v2v_to_v1v * vDirVec_v3_to_v1.z) * fScale1);

		B = D3DXVECTOR3((-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.x + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.x) * fScale1,
			(-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.y + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.y) * fScale1,
			(-vDirVec_v3u_to_v1u * vDirVec_v2_to_v1.z + vDirVec_v2u_to_v1u * vDirVec_v3_to_v1.z) * fScale1);

		// The normal N is calculated as the cross product between T and B
		D3DXVec3Cross( &N, &T, &B );

		// Calculate and cache the reciprocal value
		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) + 
			(B.x * N.y * T.z - B.z * N.y * T.x) + 
			(N.x * T.y * B.z - N.z * T.y * B.x));

		//
		// Use the temporary T (Tangent), (B) Binormal, and N (Normal) vectors 
		// to calculate the inverse of the tangent matrix that they represent.
		// The inverse of the tangent matrix is what we want since we need that
		// to transform the light's vector into tangent-space.
		//

		D3DXVECTOR3 vTemp;

		(*vTangent).x =   D3DXVec3Cross( &vTemp, &B, &N )->x * fScale2;
		(*vTangent).y = -(D3DXVec3Cross( &vTemp, &N, &T )->x * fScale2);
		(*vTangent).z =   D3DXVec3Cross( &vTemp, &T, &B )->x * fScale2;
		D3DXVec3Normalize( &(*vTangent), &(*vTangent) );

		(*vBiNormal).x = -(D3DXVec3Cross( &vTemp, &B, &N )->y * fScale2);
		(*vBiNormal).y =   D3DXVec3Cross( &vTemp, &N, &T )->y * fScale2;
		(*vBiNormal).z = -(D3DXVec3Cross( &vTemp, &T, &B )->y * fScale2);
		D3DXVec3Normalize( &(*vBiNormal), &(*vBiNormal) );

		(*vNormal).x =   D3DXVec3Cross( &vTemp, &B, &N )->z * fScale2;
		(*vNormal).y = -(D3DXVec3Cross( &vTemp, &N, &T )->z * fScale2);
		(*vNormal).z =   D3DXVec3Cross( &vTemp, &T, &B )->z * fScale2;
		D3DXVec3Normalize( &(*vNormal), &(*vNormal) );

		//
		// NOTE: Since the texture-space of Direct3D and OpenGL are laid-out 
		//       differently, a single normal map can't look right in both 
		//       unless you make some adjustments somewhere.
		//
		//       You can adjust or fix this problem in three ways:
		//
		//       1. Create two normal maps: one for OpenGL and one for Direct3D.
		//       2. Flip the normal map image over as you load it into a texture 
		//          object.
		//       3. Flip the binormal over when computing the tangent-space
		//          matrix.
		//
		// Since the normal map used by this sample was created for Direct3D,
		// no adjustment is necessary.
		//
		//*vBiNormal = *vBiNormal * -1.0f;
	}
}

//-----------------------------------------------------------------------------
// Name: encodeVectorAsDWORDColor()
// Desc: 
//-----------------------------------------------------------------------------
DWORD CBumpMappingObjet::encodeVectorAsDWORDColor( D3DXVECTOR3* vVector )
{
	DWORD dwRed   = (DWORD)(127 * vVector->x + 128);
	DWORD dwGreen = (DWORD)(127 * vVector->y + 128);
	DWORD dwBlue  = (DWORD)(127 * vVector->z + 128);

	return (DWORD)(0xff000000 + (dwRed << 16) + (dwGreen << 8) + dwBlue);
}