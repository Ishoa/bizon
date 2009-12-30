
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

// Deuxi�me constructeur de la classe CBoundingSphere.
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
    // On commande la destruction de l'objet virtuel, s'il a �t� cr�e.
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

	// On calcule, puis on inscrit le centre de la sph�re.
	m_vCenter =  D3DXVECTOR3( ( Min.x + Max.x ) / 2.0f,
							  ( Min.y + Max.y ) / 2.0f,
							  ( Min.z + Max.z ) / 2.0f );

	// Ensuite, on calcule le rayon de la sph�re.
	D3DXVECTOR3 RadiusVector = D3DXVECTOR3( ( Max.x - Min.x ) / 2.0f,
											( Max.y - Min.y ) / 2.0f,
											( Max.z - Min.z ) / 2.0f );

	m_fRadius = D3DXVec3Length( &RadiusVector );

}

// Obtient la coordonn�e du centre de la sph�re.
D3DXVECTOR3 CBoundingSphere::GetCenter()
{
    return m_vCenter;
}


// Obtient le rayon de la sph�re.
float CBoundingSphere::GetRadius()
{
    return m_fRadius;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void CBoundingSphere::SetAlphaBlending()
{
    // En d�finissant la source et la destination de l'alpha blending � ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // d�sactiv� pour cette op�ration, l'objet doit donc �tre rendu � la
    // fin de la sc�ne, �ventuellement tri� par ordre de profondeur s'il y a
    // plusieurs objets de ce type � dessiner, afin d'obtenir un affichage
    // correct.

	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
	
}

// Cette fonction cr�e un objet virtuel repr�sentant la sph�re dans le monde 3D.
bool CBoundingSphere::CreateVirtualObject(  LPDIRECT3DDEVICE9	pD3DDevice,
											unsigned char     r,
                                            unsigned char     g,
                                            unsigned char     b )
{

    // On teste si un objet virtuel a d�j� �t� cr�e.
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

    // On demande au Device Direct3D de cr�er un mesh repr�sentant une sph�re.
    if ( FAILED( D3DXCreateSphere( pD3DDevice,
                                   m_fRadius,
                                   20,
                                   20,
                                   &Sphere,
                                   NULL ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonn�es et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Sphere->CloneMeshFVF( D3DXMESH_MANAGED,
                                       D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                       pD3DDevice,
                                       &p_Sphere ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On lib�re les ressources du Mesh cr�e par Direct3D.
    Sphere->Release();

    // Ensuite, on cr�e un acc�s au VertexBuffer de la sph�re.
    LPDIRECT3DVERTEXBUFFER9 SphereVertexBuffer = NULL;

    // Puis, on tente d'obtenir les donn�es du VertexBuffer.
    if ( FAILED( p_Sphere->GetVertexBuffer( &SphereVertexBuffer ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On d�finit les variables pour lire dans le buffer.
    PBYTE            VertexData = NULL;
    DWORD            NbVertices = p_Sphere->GetNumVertices();
    DWORD            FVFSize    = D3DXGetFVFVertexSize( p_Sphere->GetFVF() );
    E_SPHERE_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / �criture.
    if ( FAILED( SphereVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donn�e du sommet.
        CurVertex = (E_SPHERE_VERTEX*)VertexData;

        // On red�finit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on d�verrouille le VertexBuffer, et on lib�re les ressources
    // utilis�es par celui-ci. La copie se trouve � l'int�rieur du Mesh.
    SphereVertexBuffer->Unlock();
    SphereVertexBuffer->Release();

    // On configure le mat�riau pour afficher la sph�re.
    p_SphereMaterial.Ambient.r = 1.0f;
    p_SphereMaterial.Ambient.g = 1.0f;
    p_SphereMaterial.Ambient.b = 1.0f;
    p_SphereMaterial.Ambient.a = 1.0f;

	DXEtats = new CDXPileEtats(pD3DDevice);
    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction d�truit l'objet virtuel.
void CBoundingSphere::ReleaseVirtualObject()
{
    // On lib�re les ressources du Mesh.
    if ( p_Sphere != NULL )
    {
        p_Sphere->Release();
        p_Sphere = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void CBoundingSphere::RenderVirtualObject(D3DXMATRIXA16     &ObjectMatrix,bool SaveState )
{
    // On teste si le Device Direct3D existe, et si l'objet a �t� correctement
    // initialis�.
	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();

    if ( pD3DDevice == NULL || p_Sphere == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'op�ration.
        return;
    }

    // On demande de sauvegarder l'�tat du Device Direct3D.
    if(SaveState)
		DXEtats->PushDeviceProperties();
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    // Puis, on configure l'alpha blending pour le rendu.
    SetAlphaBlending();

    // On supprime le Culling pour le rendu.
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // On ajoute ensuite les valeurs du centre � la matrice de transformation.
    ObjectMatrix._41 += m_vCenter.x;
    ObjectMatrix._42 += m_vCenter.y;
    ObjectMatrix._43 += m_vCenter.z;

    // Puis, on configure la matrice de monde pour cet objet.
    pD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On d�salloue le pointeur de texture, et on d�finit le mat�riau.
    pD3DDevice->SetTexture ( 0, NULL );
    pD3DDevice->SetMaterial( &p_SphereMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Sphere->DrawSubset( 0 );

    // Enfin, on restaure l'�tat d'origine du Device Direct3D.
    if(SaveState)
		DXEtats->PopDeviceProperties();
}

// Cette fonction restaure l'objet virtuel apr�s un changement de Device.
bool CBoundingSphere::RestoreVirtualObject()
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();

	LPDIRECT3DDEVICE9 pD3DDevice = DXEtats->GetDevice3D();
    // Puis, on tente de cr�er un nouvel objet, et on retourne la r�ponse.
    return CreateVirtualObject(pD3DDevice, p_R, p_G, p_B );
}
