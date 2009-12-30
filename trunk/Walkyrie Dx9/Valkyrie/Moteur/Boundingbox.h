#ifndef CBOUNDINGBOX_H
#define CBOUNDINGBOX_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "DXPileEtats.h"
#include "Calculsmaths.h"
/*
 * Cette structure permet d'accéder aux données de la boîte virtuelle.
 */
struct E_BOX_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
};

/*
 * Cette classe représente une boîte de collisions pour un objet.
 */
class CBoundingBox
{
  private:
	  	

    D3DXVECTOR3         p_Min;
    D3DXVECTOR3         p_Max;

    D3DMATERIAL9        p_BoxMaterial;

    LPD3DXMESH          p_Box;

	CDXPileEtats*		DXEtats;

    unsigned char       p_R;
    unsigned char       p_G;
    unsigned char       p_B;

    DWORD               p_NumMaterials;

  public:

    CBoundingBox();
    CBoundingBox( D3DXVECTOR3 Min, D3DXVECTOR3 Max );

    ~CBoundingBox();

  public:

    D3DXVECTOR3 GetMin              ();
    void        SetMin              ( D3DXVECTOR3 Min );
    D3DXVECTOR3 GetMax              ();
    void        SetMax              ( D3DXVECTOR3 Max );

    D3DXVECTOR3 GetCenter           ();
    E_Axis      GetLongestAxis      ();
    D3DXPLANE   GetSplittingPlane   ( E_Axis Axis );
    D3DXPLANE   GetSplittingPlane   ( E_Axis Axis, D3DXVECTOR3 Center );

    bool        CreateVirtualObject ( LPDIRECT3DDEVICE9	pD3DDevice,
									  unsigned char     r,
                                      unsigned char     g,
                                      unsigned char     b );

    void        ReleaseVirtualObject();
    void        RenderVirtualObject (D3DXMATRIXA16     &ObjectMatrix, bool SaveState = true );
    bool        RestoreVirtualObject();

  private:

    void        SetAlphaBlending    ( );
};

#endif // E_BOUNDINGBOX_H
