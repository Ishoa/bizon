#include "Calculsmaths.h"
#define RADIANS 5,5

// Constructeur de la classe CMaths.
CMaths::CMaths()
{
}

// Destructeur de la classe E_Maths.
CMaths::~CMaths()
{
}

void CMaths::CalculMatrixTransformationToXYZ(D3DXMATRIXA16* ObjectMatrix, 
											float * fAngleX, float * fAngleY,float * fAngleZ,
											float * fTranslationX, float * fTranslationY,float * fTranslationZ)
{

	float fangle_x,fangle_y,fangle_z,ftrx,ftry,fC,fD;
	fangle_y = -asin(ObjectMatrix->_13);           /* Calcul de l'Angle Y */
	fD = fangle_y;
	fC =  cos(fangle_y);


	if (fabs(fC) > 0.005)                /* Gimbal lock ? */
	{
		ftrx      =  ObjectMatrix->_33 / fC;           /* Non, donc calcul de l'angle X */
		ftry      = -ObjectMatrix->_23  / fC;

		fangle_x  = atan2(ftry, ftrx);

		ftrx      =  ObjectMatrix->_11 / fC;            /* Calcul de l'angle Z */
		ftry      = -ObjectMatrix->_12 / fC;

		fangle_z  = atan2(ftry, ftrx);
	}
	else                                   /* Gimbal lock  */
	{
		fangle_x  = 0;                      /* Angle X à 0 */

		ftrx      = ObjectMatrix->_22;                 /* Calcul de l'angle Z */
		ftry      = ObjectMatrix->_21;

		fangle_z  = atan2(ftry, ftrx);
	}

	//fangle_x = ((int)fangle_x)%360; 
	//fangle_y = ((int)fangle_y)%360;
	//fangle_z = ((int)fangle_z)%360;

	Console<<"Angle X : "<<fangle_x<<endl;
	Console<<"Angle Y : "<<fangle_y<<endl;
	Console<<"Angle Z : "<<fangle_z<<endl;
	Console<<"Translation X : "<<ObjectMatrix->_41<<endl;
	Console<<"Translation Y : "<<ObjectMatrix->_42<<endl;
	Console<<"Translation Z : "<<ObjectMatrix->_43<<endl;

	//	fTranslationX = ObjectMatrix->_41;
	//	fTranslationY = ObjectMatrix->_42;
	//	fTranslationZ = ObjectMatrix->_43;
	//  fAngleX = fangle_x;
	//	fAngleY = fangle_y;
	//	fAngleZ = fangle_z;
}

// Cette fonction retourne la valeur maximum entre deux valeurs entières.
int CMaths::GetMaxValue( int First, int Second )
{
    // On compare les deux valeurs.
    if ( First > Second )
    {
        // Si First est plus grand que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne la valeur maximum entre deux valeurs flottantes.
float CMaths::GetMaxValue( float First, float Second )
{
    // On compare les deux valeurs.
    if ( First > Second )
    {
        // Si First est plus grand que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne le vecteur maximisé de deux vecteurs.
D3DXVECTOR3 CMaths::GetMaxValue( D3DXVECTOR3 First, D3DXVECTOR3 Second )
{
    // Pour chaque valeur X, Y et Z, on recherche la valeur la plus grande,
    // puis on retourne le vecteur maximisé.
    return D3DXVECTOR3( GetMaxValue( First.x, Second.x ),
                        GetMaxValue( First.y, Second.y ),
                        GetMaxValue( First.z, Second.z ) );
}

// Cette fonction retourne la valeur minimum entre deux valeurs entières.
int CMaths::GetMinValue( int First, int Second )
{
    // On compare les deux valeurs.
    if ( First < Second )
    {
        // Si First est plus petit que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne la valeur minimum entre deux valeurs flottantes.
float CMaths::GetMinValue( float First, float Second )
{
    // On compare les deux valeurs.
    if ( First < Second )
    {
        // Si First est plus petit que Second, on retourne First.
        return First;
    }
    else
    {
        // Sinon, on retourne Second.
        return Second;
    }
}

// Cette fonction retourne le vecteur minimisé de deux vecteurs.
D3DXVECTOR3 CMaths::GetMinValue( D3DXVECTOR3 First, D3DXVECTOR3 Second )
{
    // Pour chaque valeur X, Y et Z, on recherche la valeur la plus petite,
    // puis on retourne le vecteur minimisé.
    return D3DXVECTOR3( GetMinValue( First.x, Second.x ),
                        GetMinValue( First.y, Second.y ),
                        GetMinValue( First.z, Second.z ) );
}

// Cette fonction retourne un vecteur avec des valeurs non signées.
D3DXVECTOR3 CMaths::GetUnsignedValue( D3DXVECTOR3 Value )
{
    // On teste le signe de chaque valeur, et on inverse les valeurs négatives,
    // pour les rendre positives. Ensuite, on retourne la réponse.
    return D3DXVECTOR3( Value.x < 0.0f ? -Value.x : Value.x,
                        Value.y < 0.0f ? -Value.y : Value.y,
                        Value.z < 0.0f ? -Value.z : Value.z );
}

// Cette fonction calcule, puis retourne un vecteur normalisé.
D3DXVECTOR3 CMaths::Normalize( D3DXVECTOR3 Vector )
{
    // On calcule tout d'abord la valeur de la norme du vecteur.
    float Len = sqrt( ( Vector.x * Vector.x ) +
                      ( Vector.y * Vector.y ) +
                      ( Vector.z * Vector.z ) );

    // Ensuite, on divise le vecteur par sa norme, et on retourne la réponse.
    return D3DXVECTOR3( ( Vector.x / Len ),
                        ( Vector.y / Len ),
                        ( Vector.z / Len ) );
}

// Cette fonction calcule, puis retourne la longeur d'un vecteur.
float CMaths::GetVectorLength( D3DXVECTOR3 Vector )
{
    // On calcule la longeur du vecteur, et on retourne la réponse.
    return (float)sqrt( ( Vector.x * Vector.x ) +
                        ( Vector.y * Vector.y ) +
                        ( Vector.z * Vector.z ) );
}

// Cette fonction calcule, puis retourne le produit vectoriel de deux vecteurs.
D3DXVECTOR3 CMaths::GetCrossProduct( D3DXVECTOR3 V1, D3DXVECTOR3 V2 )
{
    // On calcule le produit vectoriel des vecteurs, et on retourne la réponse.
    return D3DXVECTOR3( ( V1.y * V2.z ) - ( V2.y * V1.z ),
                        ( V1.z * V2.x ) - ( V2.z * V1.x ),
                        ( V1.x * V2.y ) - ( V2.x * V1.y ) );
}

// Cette fonction calcule, puis retourne le produit scalaire de deux vecteurs.
float CMaths::GetDotProduct( D3DXVECTOR3 V1, D3DXVECTOR3 V2 )
{
    // On calcule le produit scalaire des vecteurs, et on retourne la réponse.
    return ( ( V1.x * V2.x ) + ( V1.y * V2.y ) + ( V1.z * V2.z ) );
}

// Calcule la distance entre un point de l'espace et un plan.
float CMaths::GetDistanceToPlane( D3DXPLANE Plane, D3DXVECTOR3 Point )
{
    // On récupère la normale du plan.
    D3DXVECTOR3 N = D3DXVECTOR3( Plane.a, Plane.b, Plane.c );

    // Puis, on calcule la distance entre le plan et le point.
    return D3DXVec3Dot( &N, &Point ) + Plane.d;
}


//Calcule la distance entre 2 point
float CMaths::GetDistance(D3DXVECTOR3 V1, D3DXVECTOR3 V2)
{
	return sqrt((V2.x-V1.x)*(V2.x-V1.x) + 
				(V2.y-V1.y)*(V2.y-V1.y) + 
				(V2.z-V1.z)*(V2.z-V1.z));
}