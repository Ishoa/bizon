#ifndef CPOLYGONLIST_H
#define CPOLYGONLIST_H

#include "Calculsmaths.h"
#include "Polygon.h"
#include "Boundingbox.h"
#include "Boundingsphere.h"

// ************************ Classe CPolygonContainer *************************
// Cette classe, qui repr�sente un noeud de la liste, permet de contenir un
// polygone, ou une adresse vers un polygone si la liste est une copie d'une
// autre liste.

/*
 * Classe CPolygonContainer.
 */
class CPolygonContainer
{
  private:

    CPolygon*          p_Polygon;
    CPolygonContainer* p_Next;
    bool                p_IsCopy;

  public:

    CPolygonContainer();
    ~CPolygonContainer();

  public:

    CPolygon*          GetPolygon();
    void                SetPolygon( CPolygon* Polygon, bool IsCopy );
    CPolygonContainer* GetNext   ();
    void                SetNext   ( CPolygonContainer* Next );
};


// ************************** Classe CPolygonList ****************************
// Cette classe est une liste de polygones, utilis�e par exemple pour la
// d�tection des collisions. Elle permet un acc�s simplifi� aux donn�es
// spaciales repr�sentant un objet, et mets � disposition des outils comme la
// cr�ation d'une bo�te de collisions pour l'objet.

/*
 * Classe CPolygonList.
 */


class CPolygonList
{
  private:

    CPolygonContainer* p_First;
    CPolygonContainer* p_Current;

	CPolygonContainer* p_FirstBox;
	CPolygonContainer* p_CurrentBox;

    CBoundingBox       p_BoundingBox;
    CBoundingSphere    p_BoundingSphere;

    int                p_PolygonCount;

  public:

    CPolygonList();
    ~CPolygonList();

  public:

    bool                CreateListFromMesh  ( LPD3DXMESH Mesh );

    bool                AddPolygon          ( CPolygon* Polygon, bool IsCopy );
    bool                AddPolygon          ( D3DXVECTOR3 Vertex1,
                                              D3DXVECTOR3 Vertex2,
                                              D3DXVECTOR3 Vertex3 );

	bool                AddPolygonBox       ( CPolygon* Polygon, bool IsCopy );
	bool                AddPolygonBox       ( D3DXVECTOR3 Vertex1,
											  D3DXVECTOR3 Vertex2,
										      D3DXVECTOR3 Vertex3 );

    bool                Exist               ( CPolygon* Polygon );

    void                CreateBoundingBox   ();
	bool                CreateListBoundingBox   (D3DXVECTOR3 min,D3DXVECTOR3 max);

    CBoundingBox&        GetBoundingBox      ();

    void                CreateBoundingSphere();
    CBoundingSphere&     GetBoundingSphere   ();

    int                 GetCount            ();

    CPolygonContainer* GetFirst            ();

	CPolygonContainer*  GetFirstBox        ();

    void                ReleaseList         ();

	void				RenduPolygonList();
};

#endif // CPOLYGONLIST_H
