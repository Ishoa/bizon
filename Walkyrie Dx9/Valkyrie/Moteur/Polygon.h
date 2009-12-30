#ifndef CPOLYGON_H
#define CPOLYGON_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"


#include "Boundingbox.h"
#include "Boundingsphere.h"
#include "Calculsmaths.h"

/*
 * Cette classe représente un polygone à 3 sommets.
 */
class CPolygon
{
  private:

    D3DXVECTOR3    m_pVertex[3];
    CBoundingBox* m_pBoundingBox;
	CBoundingSphere* m_pBoundingSphere;

  public:

    CPolygon();
    CPolygon( D3DXVECTOR3 Vertex1, D3DXVECTOR3 Vertex2, D3DXVECTOR3 Vertex3 );

    ~CPolygon();

  public:

    D3DXVECTOR3    GetVertex        ( int Index );
    bool           SetVertex        ( int Index, D3DXVECTOR3 Vertex );

    D3DXVECTOR3    GetVertex1       ();
    void           SetVertex1       ( D3DXVECTOR3 Value );
    D3DXVECTOR3    GetVertex2       ();
    void           SetVertex2       ( D3DXVECTOR3 Value );
    D3DXVECTOR3    GetVertex3       ();
    void           SetVertex3       ( D3DXVECTOR3 Value );

    CBoundingBox* GetBoundingBox   ();
	CBoundingSphere* GetBoundingSphere   ();

    D3DXPLANE      GetPlane         ();
    D3DXVECTOR3    GetCenter        ();
    CPolygon      GetClone         ();
    CPolygon      ApplyMatrix      ( D3DXMATRIXA16 &Matrix );

  private:

    void           UpdateBounding();
};

#endif // CPOLYGON_H
