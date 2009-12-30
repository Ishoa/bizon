
#include "Boundingsphere.h"

// Constructeur de la classe CBoundingSphere.
CBoundingSphere::CBoundingSphere()
{
    m_vCenter           = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    m_fRadius           = 0.0f;

    p_Sphere           = NULL;

	DXEtats = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
}

// Deuxième constructeur de la classe CBoundingSphere.
CBoundingSphere::CBoundingSphere(D3DXVECTOR3 Center, float Radius )
{

    m_vCenter           = Center;
    m_fRadius           = Radius;

    p_Sphere           = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
}

// Destructeur de la classe CBoundingSphere.
CBoundingSphere::~CBoundingSphere()
{
    // On commande la destruction de l'objet virtuel, s'il a été crée.
    ReleaseVirtualObject();
}

void CBoundingSphere::SetSphere(D3DXMATRIX &ObjectMatrix, D3DXVECTOR3 Min,D3DXVECTOR3 Max)
{

// 	Min.x *= ObjectMatrix._14;
// 	Min.y *= ObjectMatrix._24;
// 	Min.z *= ObjectMatrix._34;
// 
// 
// 	Max.x *= ObjectMatrix._14;
// 	Max.y *= ObjectMatrix._24;
// 	Max.z *= ObjectMatrix._34;

	// On calcule, puis on inscrit le centre de la sphère.
	m_vCenter =  D3DXVECTOR3( ( Min.x + Max.x ) / 2.0f,
							  ( Min.y + Max.y ) / 2.0f,
							  ( Min.z + Max.z ) / 2.0f );

	// Ensuite, on calcule le rayon de la sphère.
	D3DXVECTOR3 RadiusVector = D3DXVECTOR3( ( Max.x - Min.x ) / 2.0f,
											( Max.y - Min.y ) / 2.0f,
											( Max.z - Min.z ) / 2.0f );

	m_fRadius = D3DXVec3Length( &RadiusVector );

}

// Obtient la coordonnée du centre de la sphère.
D3DXVECTOR3 CBoundingSphere::GetCenter()
{
    return m_vCenter;
}


// Obtient le rayon de la sphère.
float CBoundingSphere::GetRadius()
{
    return m_fRadius;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void CBoundingSphere::SetAlphaBlending()
{
    // En définissant la source et la destination de l'alpha blending à ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // désactivé pour cette opération, l'objet doit donc être rendu à la
    // fin de la scène, éventuellement trié par ordre de profondeur s'il y a
    // plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
    // correct.

	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
	
}

// Cette fonction crée un objet virtuel représentant la sphère dans le monde 3D.
bool CBoundingSphere::CreateVirtualObject(  LPDIRECT3DDEVICE9	pD3DDevice,
											unsigned char     r,
                                            unsigned char     g,
                                            unsigned char     b )
{

    // On teste si un objet virtuel a déjà été crée.
    if ( pD3DDevice== NULL || p_Sphere != NULL )
    {
        // Si c'est le cas, on retourne une erreur.
        return false;
    }

    // On copie les valeurs pour la couleur de l'objet.
    p_R = r;
    p_G = g;
    p_B = b;

    LPD3DXMESH Sphere = NULL;

    // On demande au Device Direct3D de créer un mesh représentant une sphère.
    if ( FAILED( D3DXCreateSphere( pD3DDevice,
                                   m_fRadius,
                                   20,
                                   20,
                                   &Sphere,
                                   NULL ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonnées et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Sphere->CloneMeshFVF( D3DXMESH_MANAGED,
                                       D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                       pD3DDevice,
                                       &p_Sphere ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On libère les ressources du Mesh crée par Direct3D.
    Sphere->Release();

    // Ensuite, on crée un accès au VertexBuffer de la sphère.
    LPDIRECT3DVERTEXBUFFER9 SphereVertexBuffer = NULL;

    // Puis, on tente d'obtenir les données du VertexBuffer.
    if ( FAILED( p_Sphere->GetVertexBuffer( &SphereVertexBuffer ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On définit les variables pour lire dans le buffer.
    PBYTE            VertexData = NULL;
    DWORD            NbVertices = p_Sphere->GetNumVertices();
    DWORD            FVFSize    = D3DXGetFVFVertexSize( p_Sphere->GetFVF() );
    E_SPHERE_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / écriture.
    if ( FAILED( SphereVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donnée du sommet.
        CurVertex = (E_SPHERE_VERTEX*)VertexData;

        // On redéfinit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on déverrouille le VertexBuffer, et on libère les ressources
    // utilisées par celui-ci. La copie se trouve à l'intérieur du Mesh.
    SphereVertexBuffer->Unlock();
    SphereVertexBuffer->Release();

    // On configure le matériau pour afficher la sphère.
    p_SphereMaterial.Ambient.r = 1.0f;
    p_SphereMaterial.Ambient.g = 1.0f;
    p_SphereMaterial.Ambient.b = 1.0f;
    p_SphereMaterial.Ambient.a = 1.0f;

	DXEtats = new CDXPileEtats(pD3DDevice);
    // Si tout s'est bien passé, on retourne le message de réussite.
    return true;
}

// Cette fonction détruit l'objet virtuel.
void CBoundingSphere::ReleaseVirtualObject()
{
    // On libère les ressources du Mesh.
    if ( p_Sphere != NULL )
    {
        p_Sphere->Release();
        p_Sphere = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void CBoundingSphere::RenderVirtualObject(D3DXMATRIXA16     &ObjectMatrix,bool SaveState )
{
    // On teste si le Device Direct3D existe, et si l'objet a été correctement
    // initialisé.
	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();

    if ( pD3DDevice == NULL || p_Sphere == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'opération.
        return;
    }

    // On demande de sauvegarder l'état du Device Direct3D.
    if(SaveState)
		DXEtats->PushDeviceProperties();
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    // Puis, on configure l'alpha blending pour le rendu.
    SetAlphaBlending();

    // On supprime le Culling pour le rendu.
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // On ajoute ensuite les valeurs du centre à la matrice de transformation.
    ObjectMatrix._41 += m_vCenter.x;
    ObjectMatrix._42 += m_vCenter.y;
    ObjectMatrix._43 += m_vCenter.z;

    // Puis, on configure la matrice de monde pour cet objet.
    pD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On désalloue le pointeur de texture, et on définit le matériau.
    pD3DDevice->SetTexture ( 0, NULL );
    pD3DDevice->SetMaterial( &p_SphereMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Sphere->DrawSubset( 0 );

    // Enfin, on restaure l'état d'origine du Device Direct3D.
    if(SaveState)
		DXEtats->PopDeviceProperties();
}

// Cette fonction restaure l'objet virtuel après un changement de Device.
bool CBoundingSphere::RestoreVirtualObject()
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();

	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();
    // Puis, on tente de créer un nouvel objet, et on retourne la réponse.
    return CreateVirtualObject(pD3DDevice, p_R, p_G, p_B );
}
