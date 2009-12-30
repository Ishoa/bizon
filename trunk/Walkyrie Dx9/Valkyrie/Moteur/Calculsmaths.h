#ifndef CMATHS_H
#define CMATHS_H


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Moteur.h"

/*
 * Enumération des axes possibles dans un système d'axes 3D.
 */

enum E_Axis
{
    E_X_AXIS     = 0,
    E_Y_AXIS     = 1,
    E_Z_AXIS     = 2
};

/*
 * Cette classe est une collection d'outils pour la résolution d'équations.
 */
class CMaths
{
  private:

    CMaths();
    ~CMaths();

  public:
	// Cette fonction retourne les coefficient de rotation et de translation d'une matrix de transformation
	static void CalculMatrixTransformationToXYZ(D3DXMATRIXA16* ObjectMatrix,
												float * fAngleX, float * fAngleY,float * fAngleZ,
												float * fTranslationX, float * fTranslationY,float * fTranslationZ);

	// Cette fonction retourne la valeur maximum entre deux valeurs entières.
    static int         GetMaxValue       ( int First, int Second );

	// Cette fonction retourne la valeur maximum entre deux valeurs flottantes.
    static float       GetMaxValue       ( float First, float Second );

    // Cette fonction retourne le vecteur maximisé de deux vecteurs.
	static D3DXVECTOR3 GetMaxValue       ( D3DXVECTOR3 First,D3DXVECTOR3 Second );

	// Cette fonction retourne la valeur minimum entre deux valeurs entières.
    static int         GetMinValue       ( int First, int Second );

	// Cette fonction retourne la valeur minimum entre deux valeurs flottantes.
    static float       GetMinValue       ( float First, float Second );

	// Cette fonction retourne le vecteur minimisé de deux vecteurs.
    static D3DXVECTOR3 GetMinValue       ( D3DXVECTOR3 First,D3DXVECTOR3 Second );

	// Cette fonction retourne un vecteur avec des valeurs non signées.
    static D3DXVECTOR3 GetUnsignedValue  ( D3DXVECTOR3 Value );

	// Cette fonction calcule, puis retourne un vecteur normalisé.
    static D3DXVECTOR3 Normalize         ( D3DXVECTOR3 Vector );

	// Cette fonction calcule, puis retourne la longeur d'un vecteur.
    static float       GetVectorLength   ( D3DXVECTOR3 Vector );

	// Cette fonction calcule, puis retourne le produit vectoriel de deux vecteurs.
    static D3DXVECTOR3 GetCrossProduct   ( D3DXVECTOR3 V1, D3DXVECTOR3 V2 );

	// Cette fonction calcule, puis retourne le produit scalaire de deux vecteurs.
    static float       GetDotProduct     ( D3DXVECTOR3 V1, D3DXVECTOR3 V2 );

	// Calcule la distance entre un point de l'espace et un plan.
    static float       GetDistanceToPlane( D3DXPLANE Plane, D3DXVECTOR3 Point );

	//Calcule la distance entre 2 point
	static float GetDistance(D3DXVECTOR3 V1, D3DXVECTOR3 V2);


};

#endif // E_MATHS_H
