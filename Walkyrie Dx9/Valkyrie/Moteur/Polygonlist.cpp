#include "Polygonlist.h"

// ************************ Classe CPolygonContainer *************************
// Cette classe, qui repr�sente un noeud de la liste, permet de contenir un
// polygone, ou une adresse vers un polygone si la liste est une copie d'une
// autre liste.

// Constructeur de la classe E_PolygonContainer.
CPolygonContainer::CPolygonContainer()
{
    p_Polygon = NULL;
    p_Next    = NULL;
    p_IsCopy  = false;
}

// Destructeur de la classe E_PolygonContainer.
CPolygonContainer::~CPolygonContainer()
{
    // On nettoie la ressource, � condition que celle-ci soit locale.
    if ( p_Polygon != NULL && p_IsCopy == false )
    {
        delete p_Polygon;
        p_Polygon = NULL;
    }
}

// Obtient le polygone, ou l'adresse du polygone.
CPolygon* CPolygonContainer::GetPolygon()
{
    return p_Polygon;
}

// D�finit le polygone, ou l'adresse du polygone.
void CPolygonContainer::SetPolygon( CPolygon* Polygon, bool IsCopy )
{
    p_Polygon = Polygon;
    p_IsCopy  = IsCopy;
}

// Obtient le pointeur vers le noeud suivant.
CPolygonContainer* CPolygonContainer::GetNext()
{
    return p_Next;
}

// D�finit le pointeur vers le noeud suivant.
void CPolygonContainer::SetNext( CPolygonContainer* Next )
{
    p_Next = Next;
}

// ************************** Classe CPolygonList ****************************
// Cette classe est une liste de polygones, utilis�e par exemple pour la
// d�tection des collisions. Elle permet un acc�s simplifi� aux donn�es
// spaciales repr�sentant un objet, et mets � disposition des outils comme la
// cr�ation d'une bo�te de collisions pour l'objet.

// Constructeur de la classe E_PolygonList.
CPolygonList::CPolygonList()
{
    p_First        = NULL;
    p_Current      = NULL;

	p_FirstBox = NULL;
	p_CurrentBox = NULL;

    p_PolygonCount = 0;
}

// Destructeur de la classe E_PolygonList.
CPolygonList::~CPolygonList()
{
    // On appelle la fonction de nettoyage de la liste.
    ReleaseList();
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool CPolygonList::AddPolygon( CPolygon* Polygon, bool IsCopy )
{
    // On commence par tester si l'objet est valide.
    if ( Polygon == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // Ensuite, on cr�e un nouveau conteneur.
    CPolygonContainer* p_New = new CPolygonContainer();

    // Puis, on teste si le conteneur a �t� correctement allou� en m�moire.
    if ( p_New == NULL )
    {
        // Si ce n'est pas le cas, on retourne une erreur.
        return false;
    }

    // On alloue ensuite le pointeur vers le polygone dans le conteneur.
    p_New->SetPolygon( Polygon, IsCopy );

    // On teste si le premier conteneur de la liste d'objets a d�j� �t� cr�e.
    if ( p_First == NULL )
    {
        // Si ce n'est pas le cas, on d�finit le nouveau conteneur comme
        // premier conteneur de la liste d'objets, et on d�finit le pointeur
        // courant sur le premier conteneur de la liste.
        p_First   = p_New;
        p_Current = p_First;
    }
    else
    {
        // Si le premier conteneur est d�fini, on ajoute le nouveau conteneur �
        // la fin de la liste. On commence par d�finir le pointeur courant sur
        // le premier conteneur.
        p_Current = p_First;

        // Puis, on passe toute la liste d'objets, jusqu'� pointer sur le
        // dernier objet de la liste.
        while ( p_Current->GetNext() != NULL )
        {
            // On passe � l'objet suivant.
            p_Current = p_Current->GetNext();
        }

        // Maintenant, on sait que p_Current contient le dernier objet de la
        // liste. On d�finit le pointeur vers le prochain conteneur sur le
        // nouveau conteneur que l'on a cr�e.
        p_Current->SetNext( p_New );
    }

    // On additionne le compteur interne de polygones.
    p_PolygonCount++;

    // Pour finir, on retourne un message de r�ussite.
    return true;
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool CPolygonList::AddPolygon( D3DXVECTOR3 Vertex1,
                                D3DXVECTOR3 Vertex2,
                                D3DXVECTOR3 Vertex3 )
{
    // On cr�e un nouveau polygone.
    CPolygon* Polygon = new CPolygon( Vertex1, Vertex2, Vertex3 );

    // Puis, on teste si le polygone a bien �t� cr�e en m�moire.
    if ( Polygon == NULL )
    {
        return false;
    }

    // Pour finir, on tente d'ajouter le polygone � la liste, et on retourne
    // la r�ponse.
    return AddPolygon( Polygon, false );
}

// Cette fonction permet de cr�er une liste depuis un objet.
bool CPolygonList::CreateListFromMesh( LPD3DXMESH Mesh )
{
    // On teste d'abord si l'objet est valide.
    if ( Mesh == NULL )
    {
        return false;
    }

    D3DXVECTOR3  Vertex[3];
    D3DXVECTOR3  CurVertex;
    WORD*        p_Indices;
    D3DXVECTOR3* p_Vertices;

    // Puis, on obtient le nombre de faces et d'index contenus dans l'objet.
    DWORD NbVertices = Mesh->GetNumVertices();
    DWORD NbFaces    = Mesh->GetNumFaces();

    // On verrouille ensuite le buffer d'index, ...
    if ( FAILED( Mesh->LockIndexBuffer( D3DLOCK_READONLY,
                                        (void**)&p_Indices ) ) )
    {
        // Si le verrouillage �choue, on retourne une erreur.
        return false;
    }

    // ... ainsi que le buffer de sommets.
    if ( FAILED( Mesh->LockVertexBuffer( D3DLOCK_READONLY,
                                         (void**)&p_Vertices ) ) )
    {
        // Si le verrouillage �choue, on retourne une erreur.
        return false;
    }

    // On extrait ensuite les donn�es n�cessaires � l'initialisation.
    for ( DWORD i = 0; i < NbFaces; i++ )
    {
        // On copie les donn�es du polygone.
        for ( DWORD j = 0; j < 3; j++ )
        {
            CurVertex = (float*)&p_Vertices[p_Indices[3*i+j]];
            Vertex[j] = CurVertex;
        }

        // Puis, on tente d'ajouter le polygone dans la liste.
        if ( AddPolygon( Vertex[0], Vertex[1], Vertex[2] ) == false )
        {
            // Si l'ajout �choue, on d�verrouille les buffers, et on retourne
            // une erreur.
            Mesh->UnlockVertexBuffer();
            Mesh->UnlockIndexBuffer();
            return false;
        }
    }

    // Puis, on d�verrouille les buffers, ...
    Mesh->UnlockVertexBuffer();
    Mesh->UnlockIndexBuffer();

    // ... et on retourne le message de r�ussite.
    return true;
}

// Cette fonction teste si un polygone existe d�j� dans la liste.
bool CPolygonList::Exist( CPolygon* Polygon )
{
    p_Current = p_First;

    // On passe toute la liste en revue.
    while ( p_Current != NULL )
    {
        // Puis, on teste si le polygone existe d�j� dans la liste.
        if ( p_Current->GetPolygon() == Polygon )
        {
            // Si c'est le cas, on retourne une r�ponse positive.
            return true;
        }

        // Sinon, on passe au prochain polygone.
        p_Current = p_Current->GetNext();
    }

    // Si aucun polygone n'est trouv�, on retourne une r�ponse n�gative.
    return false;
}

// Cette fonction permet de cr�er une bo�te de collisions pour l'objet.
void CPolygonList::CreateBoundingBox()
{
    D3DXVECTOR3 Max = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 Min = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_Current = p_First;

    // On passe toute la liste des polygones en revue.
    while ( p_Current != NULL )
    {
        // Puis, on extrait chaque sommet.
        for ( int i = 0; i < 3; i++ )
        {
            D3DXVECTOR3 v = p_Current->GetPolygon()->GetVertex( i );

            // Pour chaque sommet, on recherche si la valeur courante est
            // plus grande ou plus petite que les valeurs maximales et
            // minimales d�j� trouv�es. Si c'est le cas, on met � jour les
            // valeurs.
            Max = CMaths::GetMaxValue( Max, v );
            Min = CMaths::GetMinValue( Min, v );
        }

        // Puis, on passe au polygone suivant.
        p_Current = p_Current->GetNext();
    }

    // Enfin, on inscrit les valeurs trouv�es dans la bo�te de collisions.
    p_BoundingBox.SetMax( Max );
    p_BoundingBox.SetMin( Min );
	
	CreateListBoundingBox   (Min,Max);

}

// Obtient la bo�te de collisions de l'objet.
CBoundingBox& CPolygonList::GetBoundingBox()
{
    return p_BoundingBox;
}

// Cette fonction permet de cr�er une sph�re de collisions pour l'objet.
void CPolygonList::CreateBoundingSphere()
{
    D3DXVECTOR3 Max = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 Min = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_Current = p_First;

    // On passe toute la liste des polygones en revue.
    while ( p_Current != NULL )
    {
        // Puis, on extrait chaque sommet.
        for ( int i = 0; i < 3; i++ )
        {
            D3DXVECTOR3 v = p_Current->GetPolygon()->GetVertex( i );

            // Pour chaque sommet, on recherche si la valeur courante est
            // plus grande ou plus petite que les valeurs maximales et
            // minimales d�j� trouv�es. Si c'est le cas, on met � jour les
            // valeurs.
            Max = CMaths::GetMaxValue( Max, v );
            Min = CMaths::GetMinValue( Min, v );
        }

        // Puis, on passe au polygone suivant.
        p_Current = p_Current->GetNext();
    }

	D3DXMATRIXA16 matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	p_BoundingSphere.SetSphere(matIdentity,Min,Max);

}

// Obtient la sph�re de collisions de l'objet.
CBoundingSphere& CPolygonList::GetBoundingSphere()
{
    return p_BoundingSphere;
}

// Obtient le nombre de polygones de l'objet.
int CPolygonList::GetCount()
{
    return p_PolygonCount;
}

// Obtient le premier conteneur de polygone de la liste.
CPolygonContainer* CPolygonList::GetFirst()
{
    return p_First;
}

// Obtient le premier conteneur de polygone de la liste.
CPolygonContainer* CPolygonList::GetFirstBox()
{
	return p_FirstBox;
}


// Cette fonction permet de lib�rer les ressources de la liste.
void CPolygonList::ReleaseList()
{
    p_Current = p_First;

    // On d�truit toute la liste d'objets.
    while ( p_Current != NULL )
    {
        CPolygonContainer* p_Tmp = p_Current->GetNext();
        delete p_Current;
        p_Current = p_Tmp;
    }

    p_First   = NULL;
    p_Current = NULL;


	p_CurrentBox = p_FirstBox;

	// On d�truit toute la liste d'objets.
	while ( p_CurrentBox != NULL )
	{
		CPolygonContainer* p_Tmp = p_CurrentBox->GetNext();
		delete p_CurrentBox;
		p_CurrentBox = p_Tmp;
	}

	p_FirstBox  = NULL;
	p_CurrentBox = NULL;

}


void CPolygonList::RenduPolygonList()
{
	


}


bool CPolygonList::CreateListBoundingBox   (D3DXVECTOR3 BoxMin,D3DXVECTOR3 BoxMax)
{
	//face 1
	CPolygon* Polygon = new CPolygon( BoxMin,
									  D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMin.z),
									  D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMin.z) );
	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMin.z),
							D3DXVECTOR3(BoxMax.x,BoxMax.y,BoxMin.z) );

	if(!AddPolygonBox(Polygon,false))
		return false;

	//face 2
	Polygon = new CPolygon( D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMin.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMax.x,BoxMax.y,BoxMin.z) );
	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMax.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMax.z),
							BoxMax);

	if(!AddPolygonBox(Polygon,false))
		return false;

	//face 3
	Polygon = new CPolygon( D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z),
							BoxMax);
	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMax.z),
							BoxMax);

	if(!AddPolygonBox(Polygon,false))
		return false;

	//face 4
	Polygon = new CPolygon( BoxMin,
							D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z));

	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMax.z),
							D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z));

	if(!AddPolygonBox(Polygon,false))
		return false;

	//face 5
	Polygon = new CPolygon( BoxMin,
							D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMin.z));

	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMin.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMax.z),
							D3DXVECTOR3(BoxMax.x,BoxMin.y,BoxMin.z));

	if(!AddPolygonBox(Polygon,false))
		return false;

	//face 6
	Polygon = new CPolygon( D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMax.x,BoxMax.y,BoxMin.z),
							D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMax.z));

	if(!AddPolygonBox(Polygon,false))
		return false;

	Polygon = new CPolygon(	D3DXVECTOR3(BoxMin.x,BoxMax.y,BoxMax.z),
							D3DXVECTOR3(BoxMax.x,BoxMax.y,BoxMin.z),
							BoxMax);

	if(!AddPolygonBox(Polygon,false))
		return false;

	return true;
}



// Cette fonction permet l'ajout d'un polygone dans la liste.
bool CPolygonList::AddPolygonBox( CPolygon* Polygon, bool IsCopy )
{
	// On commence par tester si l'objet est valide.
	if ( Polygon == NULL )
	{
		// Si ce n'est pas le cas, on retourne une erreur.
		return false;
	}

	// Ensuite, on cr�e un nouveau conteneur.
	CPolygonContainer* p_New = new CPolygonContainer();

	// Puis, on teste si le conteneur a �t� correctement allou� en m�moire.
	if ( p_New == NULL )
	{
		// Si ce n'est pas le cas, on retourne une erreur.
		return false;
	}

	// On alloue ensuite le pointeur vers le polygone dans le conteneur.
	p_New->SetPolygon( Polygon, IsCopy );

	// On teste si le premier conteneur de la liste d'objets a d�j� �t� cr�e.
	if ( p_FirstBox == NULL )
	{
		// Si ce n'est pas le cas, on d�finit le nouveau conteneur comme
		// premier conteneur de la liste d'objets, et on d�finit le pointeur
		// courant sur le premier conteneur de la liste.
		p_FirstBox   = p_New;
		p_CurrentBox = p_FirstBox;
	}
	else
	{
		// Si le premier conteneur est d�fini, on ajoute le nouveau conteneur �
		// la fin de la liste. On commence par d�finir le pointeur courant sur
		// le premier conteneur.
		p_CurrentBox = p_FirstBox;

		// Puis, on passe toute la liste d'objets, jusqu'� pointer sur le
		// dernier objet de la liste.
		while ( p_CurrentBox->GetNext() != NULL )
		{
			// On passe � l'objet suivant.
			p_CurrentBox = p_CurrentBox->GetNext();
		}

		// Maintenant, on sait que p_Current contient le dernier objet de la
		// liste. On d�finit le pointeur vers le prochain conteneur sur le
		// nouveau conteneur que l'on a cr�e.
		p_CurrentBox->SetNext( p_New );
	}

	// Pour finir, on retourne un message de r�ussite.
	return true;
}

// Cette fonction permet l'ajout d'un polygone dans la liste.
bool CPolygonList::AddPolygonBox( D3DXVECTOR3 Vertex1,
							      D3DXVECTOR3 Vertex2,
							      D3DXVECTOR3 Vertex3 )
{
	// On cr�e un nouveau polygone.
	CPolygon* Polygon = new CPolygon( Vertex1, Vertex2, Vertex3 );

	// Puis, on teste si le polygone a bien �t� cr�e en m�moire.
	if ( Polygon == NULL )
	{
		return false;
	}

	// Pour finir, on tente d'ajouter le polygone � la liste, et on retourne
	// la r�ponse.
	return AddPolygonBox( Polygon, false );
}

