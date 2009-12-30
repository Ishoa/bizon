

#ifndef CPARTICLESYSTEM_H_INCLUDED
#define CPARTICLESYSTEM_H_INCLUDED

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include "AnimatedTexture.h"

// Classify Point
const int CP_FRONT   = 0;
const int CP_BACK    = 1;
const int CP_ONPLANE = 2;

// Collision Results
const int CR_BOUNCE  = 0;
const int CR_STICK   = 1;
const int CR_RECYCLE = 2;


//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

struct Plane
{
    D3DXVECTOR3 m_vNormal;           // The plane's normal
    D3DXVECTOR3 m_vPoint;            // A coplanar point within the plane
    float       m_fBounceFactor;     // Coefficient of restitution (or how bouncy the plane is)
    int         m_nCollisionResult;  // What will particles do when they strike the plane


    Plane      *m_pNext;             // Next plane in list
};


enum EEtatParticule
{
	EEtat_Principale,
	EEtat_Secondaire,
};

struct Particle
{
    D3DXVECTOR3 m_vCurPos;    // Current position of particle
    D3DXVECTOR3 m_vCurVel;    // Current velocity of particle
    float       m_fTimePassed;  // Time of creation of particle
	float		m_fAngle;
	EEtatParticule m_eEtatParticule;

	int			m_nIndiceTextureCurrentParticule;

	float		m_fPourcentColor;

	float		m_fAttributParticule1;	//attribut supplémentaire pour les redéclaration de la fonction Update
	float		m_fAttributParticule2;
	float		m_fAttributParticule3;
	float		m_fAttributParticule4;
	float		m_fAttributParticule5;

    Particle   *m_pNext;      // Next particle in list
};


// Custom vertex and FVF declaration for point sprite vertex points
struct PointVertex
{
    D3DXVECTOR3 posit;
    D3DCOLOR    color;

	enum FVF
	{
		FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
};


//-----------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//-----------------------------------------------------------------------------

// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------



class CParticleSystem
{

	typedef HRESULT (*ptrUpdate)(CParticleSystem*, float);
	
public:

    CParticleSystem(LPDIRECT3DDEVICE9 pd3dDevice);
   ~CParticleSystem(void);

    static float getRandomMinMax( float fMin, float fMax );
	static D3DXVECTOR3 getRandomVector( void );
	static int classifyPoint( D3DXVECTOR3 *vPoint, Plane *pPlane );

    void SetMaxParticles( DWORD dwMaxParticles ) { m_dwMaxParticles = dwMaxParticles; }
	DWORD GetMaxParticles( void ) { return m_dwMaxParticles; }

    void SetNumToRelease( DWORD dwNumToRelease ) { m_dwNumToRelease = dwNumToRelease; }
	DWORD GetNumToRelease( void ) { return m_dwNumToRelease; }

    void SetReleaseInterval( float fReleaseInterval ) { m_fReleaseInterval = fReleaseInterval; }
    float GetReleaseInterval( void ) { return m_fReleaseInterval; }

    void SetLifeCycle( float fLifeCycle ) { m_fLifeCycle = fLifeCycle; }
	float GetLifeCycle( void ) { return m_fLifeCycle; }

    void SetSize( float fSize ) { m_fSize = fSize; }
	float GetSize( void ) { return m_fSize; }
	float GetMaxPointSize( void ) { return m_fMaxPointSize; }

    void SetColor( D3DXCOLOR clrColor ) { m_clrColor = clrColor; }
	D3DXCOLOR GetColor( void ) { return m_clrColor; }

    void SetPosition( D3DXVECTOR3 vPosition ) { m_vPosition = vPosition; }
	D3DXVECTOR3 GetPosition( void ) { return m_vPosition; }

    void SetVelocity( D3DXVECTOR3 vVelocity ) { m_vVelocity = vVelocity; }
	D3DXVECTOR3 GetVelocity( void ) { return m_vVelocity; }

    void SetGravity( D3DXVECTOR3 vGravity ) { m_vGravity = vGravity; }
	D3DXVECTOR3 GetGravity( void ) { return m_vGravity; }

    void SetWind( D3DXVECTOR3 vWind ) { m_vWind = vWind; }
	D3DXVECTOR3 GetWind( void ) { return m_vWind; }

    void SetAirResistence( bool bAirResistence ) { m_bAirResistence = bAirResistence; }
	bool GetAirResistence( void ) { return m_bAirResistence; }

    void SetVelocityVar( float fVelocityVar ) { m_fVelocityVar = fVelocityVar; }
	float GetVelocityVar( void ) { return m_fVelocityVar; }

	void SetCoeffRotationParticule(float fCoeffRotationParticule){m_fCoeffRotationParticule = fCoeffRotationParticule;}
	float GetCoeffRotationParticule(){return m_fCoeffRotationParticule;}

    void SetCollisionPlane( D3DXVECTOR3 vPlaneNormal, D3DXVECTOR3 vPoint, 
                            float fBounceFactor = 1.0f, int nCollisionResult = CR_BOUNCE );

    HRESULT Init();
    HRESULT Update( float fElapsedTime );
    HRESULT Rendu3D();

    HRESULT SetTexture(char *chTexFile );
	HRESULT SetMultiTexture(char** chTexFile, int nNbTexture);

	HRESULT SetTextureSecondaire(char *chTexFile );
	HRESULT SetMultiTextureSecondaire(char** chTexFile, int nNbTexture);

    HRESULT RestoreDeviceObjects();
    HRESULT Release(void);
    
	void RestartParticleSystem(void);

	void SetFonctionUpdate(ptrUpdate pUpdateFonction){m_pUpdateFonction = pUpdateFonction;}
	
	static int getNbParticule(){return m_nCompteurParticule;}
	static void ResetComteurParticule(){m_nCompteurParticule = 0;}

public:

	ptrUpdate m_pUpdateFonction;

	static int m_nCompteurParticule;

    DWORD       m_dwVBOffset;
    DWORD       m_dwFlush;
    DWORD       m_dwDiscard;
    Particle   *m_pActiveList;
    Particle   *m_pFreeList;
    Plane      *m_pPlanes;
	DWORD       m_dwActiveCount;

	float       m_fDeltatime;
	float       m_fLastUpdate;
	float		m_fOscilateSize;
	float		m_fCoeffRotationParticule;

    float       m_fMaxPointSize;
    bool        m_bDeviceSupportsPSIZE;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;          // Vertex buffer for point sprites
	LPDIRECT3DDEVICE9 m_pd3dDevice;

    CAnimatedTexture*      m_pTexPrincipale;		// Particle's texture EEtat_Normal
	CAnimatedTexture*	   m_pTexSecondaire;


	//int m_nNbTexture;

    int m_nLastIdTexture;
    // Particle Attributes
    DWORD       m_dwMaxParticles;

    DWORD       m_dwNumToRelease;
    float       m_fReleaseInterval;

    float       m_fLifeCycle;
    float       m_fSize;
    D3DXCOLOR   m_clrColor;
    D3DXVECTOR3 m_vPosition;

    D3DXVECTOR3 m_vVelocity;
    D3DXVECTOR3 m_vGravity;
    D3DXVECTOR3 m_vWind;
    bool        m_bAirResistence;
    float       m_fVelocityVar;

};

#endif /* CPARTICLESYSTEM_H_INCLUDED */