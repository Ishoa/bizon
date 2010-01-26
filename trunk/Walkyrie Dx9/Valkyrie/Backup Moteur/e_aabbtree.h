/*****************************************************************************
 * ==> Classe E_AABBTree ----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de diminuer le nombre de polygones pour *
 *               un test de collisions, en classant les polygones dans un    *
 *               arbre. L'algorythme utilisé est appelé Aligned Axis         *
 *               Bounding Box.                                               *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_AABBTREE_H
#define E_AABBTREE_H

#include "e_maths.h"
#include "e_boundingbox.h"
#include "e_boundingsphere.h"
#include "e_polygonlist.h"
#include "e_collisions.h"

// Prototype de la classe E_AABBTree.
class E_AABBTree;

/*
 * Enumérateur des algorythmes de division disponibles.
 */
enum E_DivisionType
{
    E_UNKNOWDIVISIONTYPE = 0,
    E_BESTAXIS           = 1,
    E_LONGESTAXIS        = 2
};

// ************************** Classe E_AABBTreeInfo ***************************
// Cette classe contient les informations pour le comportement de l'arbre. On
// peut y définir la profondeur maximum, ou le nombre minimum de polygones par
// feuille, par exemple.

/*
 * Classe E_AABBTreeInfo.
 */
class E_AABBTreeInfo
{
  private:

    int            p_MaxTreeDeep;
    int            p_CurrentDeep;
    int            p_MinVertices;

    E_DivisionType p_DivisionType;

  public:

    E_AABBTreeInfo();
    ~E_AABBTreeInfo();

  public:

    int            GetMaxTreeDeep ();
    void           SetMaxTreeDeep ( int MaxTreeDepth );
    int            GetMinVertices ();
    void           SetMinVertices ( int MinVertices );
    E_DivisionType GetDivisionType();
    void           SetDivisionType( E_DivisionType DivisionType );

    void           AddDeep        ();
    int            GetDeep        ();

    bool           TestDeep       ();
    bool           TestMinVertices( int CurVertices );
};

// **************************** Classe E_AABBNode *****************************
// Cette classe représente soit un noeud, soit une extrémité, aussi désignée
// par le terme feuille, dans l'arbre.

/*
 * Classe E_AABBNode.
 */
class E_AABBNode
{
  private:

    E_AABBNode*    p_Left;
    E_AABBNode*    p_Right;

    CPolygonList* p_PolygonList;

    D3DXPLANE      p_SplittingPlane;

  public:

    E_AABBNode();
    ~E_AABBNode();

  public:

    bool        CreateChild             ( CPolygonList* PolygonList,
                                          E_AABBTreeInfo Info,
                                          E_AABBTree*    Root );
    bool        Resolve                 ( CPolygonList* ResultList,
                                          CBoundingSphere Sphere );

  private:

    D3DXVECTOR3 FindPolygonsCenter      ();
    E_Axis      FindLongestAxis         ();
    E_Axis      FindBestAxis            ();
    E_Side      TestPolygonWithBBoxPlane( CPolygon* Polygon,
                                          D3DXPLANE  BBPlane );
    bool        AddPolygons             ( CPolygonList* ResultList );
};

// **************************** Classe E_AABBTree *****************************
// Cette classe représente l'arbre de tri des données. L'algorythme utilisé est
// de type Aligned Axis Bounding Box.

/*
 * Classe E_AABBTree.
 */
class E_AABBTree
{
    friend class E_AABBNode;

  private:

    E_AABBNode*    p_Root;
    E_AABBTreeInfo p_Info;

    int            p_Deep;
    int            p_ChildCount;

  public:

    E_AABBTree();
    ~E_AABBTree();

  public:

    int  GetChildCount  ();
    int  GetDeep        ();

    void SetTreeBehavior( E_AABBTreeInfo Info );
    bool Initialize     ( CPolygonList* PolygonList );
    bool Resolve        ( CPolygonList* Result, CBoundingSphere Sphere );

  private:

    void AddChildCount  ();
    void SetDeep        ( int Deep );
};

#endif // E_AABBTREE_H
