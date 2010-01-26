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

// Deuxième constructeur de la classe CBoundingBox.
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
    // On commande la destruction de l'objet virtuel, s'il a été crée.
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
	// On libère les ressources utilisées par la liste de polygones.
	if ( m_pPolygonList != NULL )
	{
		delete m_pPolygonList;
		m_pPolygonList = NULL;
	}
}
*/


// Obtient la coordonnée du coin inférieur de la boîte.
D3DXVECTOR3 CBoundingBox::GetMin()
{
    return p_Min;
}

// Définit la coordonnée du coin inférieur de la boîte.
void CBoundingBox::SetMin( D3DXVECTOR3 Min )
{
    p_Min = Min;
}

// Obtient la coordonnée du coin supérieur de la boîte.
D3DXVECTOR3 CBoundingBox::GetMax()
{
    return p_Max;
}

// Définit la coordonnée du coin supérieur de la boîte.
void CBoundingBox::SetMax( D3DXVECTOR3 Max )
{
    p_Max = Max;
}

// Obtient le point central de la boîte de collisions.
D3DXVECTOR3 CBoundingBox::GetCenter()
{
    return D3DXVECTOR3( ( p_Min.x + p_Max.x ) / 2.0f,
                        ( p_Min.y + p_Max.y ) / 2.0f,
                        ( p_Min.z + p_Max.z ) / 2.0f );
}

// Cette fonction permet de déterminer l'axe le plus long.
E_Axis CBoundingBox::GetLongestAxis()
{
    // On calcule d'abord la longeur de chaque côté.
    float x = p_Max.x - p_Min.x;
    float y = p_Max.y - p_Min.y;
    float z = p_Max.z - p_Min.z;

    // Puis, on s'assure qu'il n'y ait pas de valeurs négatives.
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    z = z < 0.0f ? -z : z;

    // On teste ensuite quel côté est le plus long.
    if ( x >= y && x >= z )
    {
        // Si c'est X, on retourne la réponse.
        return E_X_AXIS;
    }
    else if ( y >= x && y >= z )
    {
        // Si c'est Y, on retourne la réponse.
        return E_Y_AXIS;
    }
    else
    {
        // Si c'est Z, on retourne la réponse.
        return E_Z_AXIS;
    }
}

// Cette fonction permet de créer un plan de coupe sur un axe donné.
D3DXPLANE CBoundingBox::GetSplittingPlane( E_Axis Axis )
{
    // On détermine quel axe a été choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -GetCenter().x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -GetCenter().y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -GetCenter().z );
        }

        // Le choix est indéfini.
        default:
        {
            // On retourne un plan vide.
            return D3DXPLANE( 0.0f, 0.0f, 0.0f, 0.0f );
        }
    }
}

// Cette fonction crée un plan de coupe sur un axe depuis un point donné.
D3DXPLANE CBoundingBox::GetSplittingPlane( E_Axis Axis, D3DXVECTOR3 Center )
{
    // On détermine quel axe a été choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -Center.x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -Center.y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On crée, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -Center.z );
        }

        // Le choix est indéfini.
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
    // En définissant la source et la destination de l'alpha blending à ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // désactivé pour cette opération, l'objet doit donc être rendu à la
    // fin de la scène, éventuellement trié par ordre de profondeur s'il y a
    // plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
    // correct.
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
	if ( pD3DDevice == NULL) 
		return;

	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
	pD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );


}


// Cette fonction crée un objet virtuel représentant la boîte dans le monde 3D.
bool CBoundingBox::CreateVirtualObject(  LPDIRECT3DDEVICE9	pD3DDevice,
									     unsigned char     r,
                                         unsigned char     g,
                                         unsigned char     b )
{

    // On teste si un objet virtuel a déjà été crée.
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

    // On demande au Device Direct3D de créer un mesh représentant une boîte.
    if ( FAILED( D3DXCreateBox( pD3DDevice,
                                p_Max.x - p_Min.x,
                                p_Max.y - p_Min.y,
                                p_Max.z - p_Min.z,
                                &Box,
                                NULL ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonnées et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Box->CloneMeshFVF( D3DXMESH_MANAGED,
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                    pD3DDevice,
                                    &p_Box ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On libère les ressources du Mesh crée par Direct3D.
    Box->Release();

    // Ensuite, on crée un accès au VertexBuffer de la boîte.
    LPDIRECT3DVERTEXBUFFER9 BoxVertexBuffer = NULL;

    // Puis, on tente d'obtenir les données du VertexBuffer.
    if ( FAILED( p_Box->GetVertexBuffer( &BoxVertexBuffer ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On définit les variables pour lire dans le buffer.
    PBYTE         VertexData = NULL;
    DWORD         NbVertices = p_Box->GetNumVertices();
    DWORD         FVFSize    = D3DXGetFVFVertexSize( p_Box->GetFVF() );
    E_BOX_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / écriture.
    if ( FAILED( BoxVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'opération échoue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donnée du sommet.
        CurVertex = (E_BOX_VERTEX*)VertexData;

        // On redéfinit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on déverrouille le VertexBuffer, et on libère les ressources
    // utilisées par celui-ci. La copie se trouve à l'intérieur du Mesh.
    BoxVertexBuffer->Unlock();
    BoxVertexBuffer->Release();

    // On configure le matériau pour afficher la boîte.
    p_BoxMaterial.Ambient.r = 1.0f;
    p_BoxMaterial.Ambient.g = 1.0f;
    p_BoxMaterial.Ambient.b = 1.0f;
    p_BoxMaterial.Ambient.a = 1.0f;

	DXEtats = new CDXPileEtats(pD3DDevice);
    // Si tout s'est bien passé, on retourne le message de réussite.
    return true;
}

// Cette fonction détruit l'objet virtuel.
void CBoundingBox::ReleaseVirtualObject()
{
    // On libère les ressources du Mesh.
    if ( p_Box != NULL )
    {
        p_Box->Release();
        p_Box = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void CBoundingBox::RenderVirtualObject(D3DXMATRIXA16     &ObjectMatrix ,bool SaveState)
{
    // On teste si le Device Direct3D existe, et si l'objet a été correctement
    // initialisé.
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
    if ( pD3DDevice == NULL) 
        return;

	if( p_Box == NULL )
		return;

    // On demande de sauvegarder l'état du Device Direct3D.
   if(SaveState) 
	   DXEtats->PushDeviceProperties();

	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    // Puis, on configure l'alpha blending pour le rendu.
    SetAlphaBlending();

    // On supprime le Culling pour le rendu.
    pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    D3DXVECTOR3 Center = GetCenter();

    // On ajoute ensuite les valeurs du centre à la matrice de transformation.
    ObjectMatrix._41 += Center.x;
    ObjectMatrix._42 += Center.y;
    ObjectMatrix._43 += Center.z;

    // Puis, on configure la matrice de monde pour cet objet.
    pD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On désalloue le pointeur de texture, et on définit le matériau.
    pD3DDevice->SetTexture ( 0, NULL );
    pD3DDevice->SetMaterial( &p_BoxMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Box->DrawSubset( 0 );

    // Enfin, on restaure l'état d'origine du Device Direct3D.
    if(SaveState)
		DXEtats->PopDeviceProperties();
}

// Cette fonction restaure l'objet virtuel après un changement de Device.
bool CBoundingBox::RestoreVirtualObject()
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();
	LPDIRECT3DDEVICE9 pD3DDevice =  DXEtats->GetDevice3D();
    // Puis, on tente de créer un nouvel objet, et on retourne la réponse.
    return CreateVirtualObject(pD3DDevice,p_R, p_G, p_B );
}
