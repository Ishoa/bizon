
#ifndef CBOUNDINGSPHERE_H
#define CBOUNDINGSPHERE_H

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Calculsmaths.h"
#include "DXPileEtats.h"

/*
 * Cette structure permet d'accéder aux données de la boîte virtuelle.
 */
struct E_SPHERE_VERTEX
{
    D3DXVECTOR3 Position;
    D3DCOLOR    Color;
};

/*
 * Cette classe représente une sphère de collisions pour un objet.
 */
class CBoundingSphere
{
    D3DXVECTOR3         p_Center;
    float               p_Radius;

    D3DMATERIAL9        p_SphereMaterial;

    LPD3DXMESH          p_Sphere;

	CDXPileEtats*		DXEtats;

    unsigned char       p_R;
    unsigned char       p_G;
    unsigned char       p_B;

    DWORD               p_NumMaterials;

  public:

    CBoundingSphere();
    CBoundingSphere( D3DXVECTOR3 Center, float Radius );

    ~CBoundingSphere();

  public:

    void        SetCenter           ( D3DXVECTOR3 Center );
    D3DXVECTOR3 GetCenter           ();
    void        SetRadius           ( float Radius );
    float       GetRadius           ();

    bool        CreateVirtualObject (  LPDIRECT3DDEVICE9	pD3DDevice,
									   unsigned char r,
									   unsigned char g, 
									   unsigned char b );

    void        ReleaseVirtualObject();
    void        RenderVirtualObject (D3DXMATRIXA16     &ObjectMatrix ,bool SaveState = true);

    bool        RestoreVirtualObject();

  private:

    void        SetAlphaBlending    ();
};

#endif // CBOUNDINGSPHERE_H
