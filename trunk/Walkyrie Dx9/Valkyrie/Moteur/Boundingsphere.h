
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
    D3DXVECTOR3         m_vCenter;
    float               m_fRadius;

	D3DXVECTOR3         m_vMin;
	D3DXVECTOR3         m_vMax;

    D3DMATERIAL9        p_SphereMaterial;

    LPD3DXMESH          p_Sphere;

	CDXPileEtats*		DXEtats;

    unsigned char       p_R;
    unsigned char       p_G;
    unsigned char       p_B;

  public:

    CBoundingSphere();
    CBoundingSphere( D3DXVECTOR3 Center, float Radius );

    ~CBoundingSphere();

  public:
	void		SetCenter			(D3DXVECTOR3 center){m_vCenter = center;}
	void		SetRadius			(float fRadius){m_fRadius = fRadius;}

	void		SetSphere			(D3DXMATRIX &ObjectMatrix, D3DXVECTOR3 p_Min ,D3DXVECTOR3 p_Max);

    D3DXVECTOR3 GetCenter           ();
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
