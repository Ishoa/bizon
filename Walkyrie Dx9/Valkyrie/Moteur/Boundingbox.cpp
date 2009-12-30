#include "Boundingbox.h"

// Constructeur de la classe CBoundingBox.
CBoundingBox::CBoundingBox()
{
    p_Min              = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Max              = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_Box              = NULL;

	DXEtats = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
    p_NumMaterials     = 0;
}

// Deuxi�me constructeur de la classe CBoundingBox.
CBoundingBox::CBoundingBox(D3DXVECTOR3 Min, D3DXVECTOR3 Max )
{
    p_Min              = Min;
    p_Max              = Max;

    p_Box              = NULL;

	DXEtats = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
    p_NumMaterials     = 0;

}

// Destructeur de la classe CBoundingBox.
CBoundingBox::~CBoundingBox()
{
    // On commande la destruction de l'objet virtuel, s'il a �t� cr�e.
    //ReleaseVirtualObject();
/*
	
	if(m_pPolygonList != NULL)
	{
		m_pPolygonList->ReleaseList();
	}
*/

}

/*

CPolygonList*   CBoundingBox::GetPolygonList   ()
{
	// On lib�re les ressources utilis�es par la liste de polygones.
	if ( m_pPolygonList != NULL )
	{
		delete m_pPolygonList;
		m_pPolygonList = NULL;
	}
}
*/


// Obtient la coordonn�e du coin inf�rieur de la bo�te.
D3DXVECTOR3 CBoundingBox::GetMin()
{
    return p_Min;
}

// D�finit la coordonn�e du coin inf�rieur de la bo�te.
void CBoundingBox::SetMin( D3DXVECTOR3 Min )
{
    p_Min = Min;
}

// Obtient la coordonn�e du coin sup�rieur de la bo�te.
D3DXVECTOR3 CBoundingBox::GetMax()
{
    return p_Max;
}

// D�finit la coordonn�e du coin sup�rieur de la bo�te.
void CBoundingBox::SetMax( D3DXVECTOR3 Max )
{
    p_Max = Max;
}

// Obtient le point central de la bo�te de collisions.
D3DXVECTOR3 CBoundingBox::GetCenter()
{
    return D3DXVECTOR3( ( p_Min.x + p_Max.x ) / 2.0f,
                        ( p_Min.y + p_Max.y ) / 2.0f,
                        ( p_Min.z + p_Max.z ) / 2.0f );
}

// Cette fonction permet de d�terminer l'axe le plus long.
E_Axis CBoundingBox::GetLongestAxis()
{
    // On calcule d'abord la longeur de chaque c�t�.
    float x = p_Max.x - p_Min.x;
    float y = p_Max.y - p_Min.y;
    float z = p_Max.z - p_Min.z;

    // Puis, on s'assure qu'il n'y ait pas de valeurs n�gatives.
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    z = z < 0.0f ? -z : z;

    // On teste ensuite quel c�t� est le plus long.
    if ( x >= y && x >= z )
    {
        // Si c'est X, on retourne la r�ponse.
        return E_X_AXIS;
    }
    else if ( y >= x && y >= z )
    {
        // Si c'est Y, on retourne la r�ponse.
        return E_Y_AXIS;
    }
    else
    {
        // Si c'est Z, on retourne la r�ponse.
        return E_Z_AXIS;
    }
}

// Cette fonction permet de cr�er un plan de coupe sur un axe donn�.
D3DXPLANE CBoundingBox::GetSplittingPlane( E_Axis Axis )
{
    // On d�termine quel axe a �t� choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -GetCenter().x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -GetCenter().y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -GetCenter().z );
        }

        // Le choix est ind�fini.
        default:
        {
            // On retourne un plan vide.
            return D3DXPLANE( 0.0f, 0.0f, 0.0f, 0.0f );
        }
    }
}

// Cette fonction cr�e un plan de coupe sur un axe depuis un point donn�.
D3DXPLANE CBoundingBox::GetSplittingPlane( E_Axis Axis, D3DXVECTOR3 Center )
{
    // On d�termine quel axe a �t� choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -Center.x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -Center.y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -Center.z );
        }

        // Le choix est ind�fini.
        default:
        {
            // On retourne un plan vide.
            return D3DXPLANE( 0.0f, 0.0f, 0.0f, 0.0f );
        }
    }
}

// Cette fonction configure l'affichage pour permettre la transparence.
void CBoundingBox::SetAlphaBlending()
{
    // En d�finissant la source et la destination de l'alpha blending � ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // d�sactiv� pour cette op�ration, l'objet doit donc �tre rendu � la
    // fin de la sc�ne, �ventuellement tri� par ordre de profondeur s'il y a
    // plusieurs objets de ce type � dessiner, afin d'obtenir un affichage
    // correct.
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
	if ( pD3DDevice == NULL) 
		return;

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );


}


// Cette fonction cr�e un objet virtuel repr�sentant la bo�te dans le monde 3D.
bool CBoundingBox::CreateVirtualObject(  LPDIRECT3DDEVICE9	pD3DDevice,
									     unsigned char     r,
                                         unsigned char     g,
                                         unsigned char     b )
{

    // On teste si un objet virtuel a d�j� �t� cr�e.
    if (pD3DDevice == NULL || p_Box != NULL )
    {
        // Si c'est le cas, on retourne une erreur.
        return false;
    }

    // On copie les valeurs pour la couleur de l'objet.
    p_R = r;
    p_G = g;
    p_B = b;

    LPD3DXMESH Box = NULL;

    // On demande au Device Direct3D de cr�er un mesh repr�sentant une bo�te.
    if ( FAILED( D3DXCreateBox( pD3DDevice,
                                p_Max.x - p_Min.x,
                                p_Max.y - p_Min.y,
                                p_Max.z - p_Min.z,
                                &Box,
                                NULL ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonn�es et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Box->CloneMeshFVF( D3DXMESH_MANAGED,
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                    pD3DDevice,
                                    &p_Box ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On lib�re les ressources du Mesh cr�e par Direct3D.
    Box->Release();

    // Ensuite, on cr�e un acc�s au VertexBuffer de la bo�te.
    LPDIRECT3DVERTEXBUFFER9 BoxVertexBuffer = NULL;

    // Puis, on tente d'obtenir les donn�es du VertexBuffer.
    if ( FAILED( p_Box->GetVertexBuffer( &BoxVertexBuffer ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On d�finit les variables pour lire dans le buffer.
    PBYTE         VertexData = NULL;
    DWORD         NbVertices = p_Box->GetNumVertices();
    DWORD         FVFSize    = D3DXGetFVFVertexSize( p_Box->GetFVF() );
    E_BOX_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / �criture.
    if ( FAILED( BoxVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donn�e du sommet.
        CurVertex = (E_BOX_VERTEX*)VertexData;

        // On red�finit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on d�verrouille le VertexBuffer, et on lib�re les ressources
    // utilis�es par celui-ci. La copie se trouve � l'int�rieur du Mesh.
    BoxVertexBuffer->Unlock();
    BoxVertexBuffer->Release();

    // On configure le mat�riau pour afficher la bo�te.
    p_BoxMaterial.Ambient.r = 1.0f;
    p_BoxMaterial.Ambient.g = 1.0f;
    p_BoxMaterial.Ambient.b = 1.0f;
    p_BoxMaterial.Ambient.a = 1.0f;

	DXEtats = new CDXPileEtats(pD3DDevice);
    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction d�truit l'objet virtuel.
void CBoundingBox::ReleaseVirtualObject()
{
    // On lib�re les ressources du Mesh.
    if ( p_Box != NULL )
    {
        p_Box->Release();
        p_Box = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void CBoundingBox::RenderVirtualObject(D3DXMATRIXA16     &ObjectMatrix ,bool SaveState)
{
    // On teste si le Device Direct3D existe, et si l'objet a �t� correctement
    // initialis�.
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
    if ( pD3DDevice == NULL) 
        return;

	if( p_Box == NULL )
		return;

    // On demande de sauvegarder l'�tat du Device Direct3D.
   if(SaveState) 
	   DXEtats->PushDeviceProperties();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    // Puis, on configure l'alpha blending pour le rendu.
    SetAlphaBlending();

    // On supprime le Culling pour le rendu.
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    D3DXVECTOR3 Center = GetCenter();

    // On ajoute ensuite les valeurs du centre � la matrice de transformation.
    ObjectMatrix._41 += Center.x;
    ObjectMatrix._42 += Center.y;
    ObjectMatrix._43 += Center.z;

    // Puis, on configure la matrice de monde pour cet objet.
    pD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On d�salloue le pointeur de texture, et on d�finit le mat�riau.
    pD3DDevice->SetTexture ( 0, NULL );
    pD3DDevice->SetMaterial( &p_BoxMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Box->DrawSubset( 0 );

    // Enfin, on restaure l'�tat d'origine du Device Direct3D.
    if(SaveState)
		DXEtats->PopDeviceProperties();
}

// Cette fonction restaure l'objet virtuel apr�s un changement de Device.
bool CBoundingBox::RestoreVirtualObject()
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
    // Puis, on tente de cr�er un nouvel objet, et on retourne la r�ponse.
    return CreateVirtualObject(pD3DDevice,p_R, p_G, p_B );
}
