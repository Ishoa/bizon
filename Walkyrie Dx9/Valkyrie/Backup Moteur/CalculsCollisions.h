#ifndef CCOLLISIONS_H
#define CCOLLISIONS_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Calculsmaths.h"
#include "Polygon.h"
#include "Boundingbox.h"
#include "Boundingsphere.h"

/*
 * Enumérateur des résultats possibles des tests de plans.
 */
enum E_Side
{
    E_FRONT    = 0,
    E_BACK     = 1,
    E_SPLITTED = 2,
    E_COPLANAR = 3,
};

// **************************** CCalculsCollisions *****************************
// Cette classe contient des outils pour faciliter le calcul d'une collision,
// ou pour calculer une collision entre deux objets simples.

/*
 * Cette classe contient des outils pour calculer une collisions.
 */
class CCalculsCollisions
{

  public:

	enum eCollisionType
	{
		NO_COLLISION          = 0,
		COLLISION_ONLY        = 1,
		COLLISION_AND_SLIDING = 2
	};


  private:

    CCalculsCollisions();
    ~CCalculsCollisions();

  public:
	// Calcule la distance entre un point de l'espace et un plan.
	static float       GetDistanceToPlane    ( D3DXPLANE   thePlane,
											   D3DXVECTOR3 thePoint );
	
	// Calcule la prochaine position valide par rapport au plan de collision.
	static D3DXVECTOR3 GetSlidingPoint       ( D3DXPLANE   SlidingPlane,
											   D3DXVECTOR3 Position,
											   float       Radius );

	// Retourne le point transformé selon l'inverse de la matrice donnée.
    static D3DXVECTOR3 GetInversedPosition   ( D3DXVECTOR3   Position,
                                               D3DXMATRIXA16 &Matrix );

	/// Teste si un point de l'espace se trouve à l'intérieur d'un polygone.
    static bool        IsPointIntoTriangle   ( D3DXVECTOR3 Point,
                                               CPolygon Polygon );
	static bool		   IsPointIntoTriangle	 ( D3DXVECTOR3 Point,
											   D3DXVECTOR3 P1,D3DXVECTOR3 P2,D3DXVECTOR3 P3);

	/// Teste si un point de l'espace se trouve à l'intérieur d'une sphère.
    static bool        IsPointIntoSphere     ( D3DXVECTOR3 Point,
                                               D3DXVECTOR3 SphereOrigin,
                                               float       SphereRadius );

	// Cette fonction détermine si une sphère croise un plan.
    static bool        SphereIntersectPlane  ( D3DXVECTOR3 Center,
                                               float       Radius,
                                               D3DXPLANE   Plane );

	// Cette fonction détermine si deux sphères se touchent.
    static bool        SphereIntersectSphere ( D3DXVECTOR3 Sphere1Center,
                                               float       Sphere1Radius,
                                               D3DXVECTOR3 Sphere2Center,
                                               float       Sphere2Radius );

	static bool        SphereIntersectSphere (  D3DXVECTOR3 Sphere1Center,
												float       Sphere1Radius,
												CBoundingSphere BoundingSphere);

	// Cette fonction détermine si une sphère croise une boîte.
    static bool        SphereIntersectBox    ( D3DXVECTOR3 SphereCenter,
                                               float       SphereRadius,
                                               D3DXVECTOR3 BoxMin,
                                               D3DXVECTOR3 BoxMax);

	static bool SphereIntersectBox(D3DXVECTOR3 SphereCenter,
									float       SphereRadius,
									CBoundingBox BoundingBox);


	// Cette fonction détermine si deux boites se touchent.
	static bool        BoxIntersectBox    ( D3DXVECTOR3 Box1Min,
											D3DXVECTOR3 Box1Max ,
											D3DXVECTOR3 Box2Min,
											D3DXVECTOR3 Box2Max );

	// Calcule la coordonnée projetée d'un point sur un segment de droite.
    static D3DXVECTOR3 ClosestPointOnLine    ( D3DXVECTOR3 SegmentStart,
                                               D3DXVECTOR3 SegmentEnd,
                                               D3DXVECTOR3 Point );


	// Calcule la coordonnée projetée d'un point sur un Triangle / polygone.
    static D3DXVECTOR3 ClosestPointOnTriangle( D3DXVECTOR3 Point,
											   CPolygon   Polygon );
	static D3DXVECTOR3 ClosestPointOnTriangle( D3DXVECTOR3 Point,
											   D3DXVECTOR3 P1,D3DXVECTOR3 P2,D3DXVECTOR3 P3);

	// Cette fonction teste si une sphère est en collision avec un Triangle / polygone.
	static bool Check( D3DXVECTOR3 Position,
					   float       Radius,
					   CPolygon   Polygon,
					   D3DXPLANE*  SlidePlane  );
	
	static bool Check( D3DXVECTOR3 Position,
					   float       Radius,
					   D3DXVECTOR3 P1,D3DXVECTOR3 P2,D3DXVECTOR3 P3,
					   D3DXPLANE*  SlidePlane );


};

#endif // CCOLLISIONS_H
