#include "Joystick.h"


CPeripherique* perif=NULL;
LPDIRECTINPUTDEVICE8 joy=NULL;


/******************* enumération des joystiques présents **********************/
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,VOID* pContext )
{
    // on obtient une interface vers le joystick énuméré.
	if(perif->GetDirectInput()->CreateDevice( pdidInstance->guidInstance, &joy, NULL)!=DI_OK)
		return DIENUM_CONTINUE;

    return DIENUM_STOP;
}

/*############################################################################*/


CJoystick::CJoystick()
{
}

CJoystick::CJoystick(CPeripherique* pPeripherique)
{
	m_pPeripherique = pPeripherique;
	perif=m_pPeripherique;

	pJoystick = NULL;
	joy=pJoystick;
}

CJoystick::~CJoystick()
{
	Console << "Deconstructeur CJoystick()" << endl;
}


// Création de la souris
bool CJoystick::Creation(LPDIRECTINPUT8 lpDI)
{
	// Appel d'un fonction callback pour énumérer les joysticks installés et branchés
    if( lpDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY )!=DI_OK )
	{
		Console << "Creation CJoystick : EnumDevices échoué" << endl;
        return FALSE;
	}
	pJoystick=joy;
	// aucain joystick installé et branché
    if( NULL == pJoystick )
    {
		Console << "Creation CJoystick échoué : aucun joystick branché" << endl;
        return FALSE;
    }

	// définition du format de donné
    if( pJoystick->SetDataFormat( &c_dfDIJoystick2 )!=DI_OK )
        return FALSE;

	// définition du niveau de coopération
	if( pJoystick->SetCooperativeLevel( m_pPeripherique->GetMoteur()->GetFenetre(), DISCL_EXCLUSIVE | DISCL_FOREGROUND )!=DI_OK)
        return FALSE;
	
	
	DIPROPRANGE diprg; // structure qui contient les propriétés de l'étendue sur l'axe X et Y
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYOFFSET;	// pour utiliser un format de donné pour dwObj	 
    diprg.diph.dwObj        = DIJOFS_X; // axe à configuré (X)
    diprg.lMin              = -12;	// étendue sur l'axe X
    diprg.lMax              = +12;	// (nombre de points de contrôles)

	// on affecte ces propriétés à l'interface du joystick
	if(pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph )!=DI_OK)
		return FALSE;
	
	// configure l'axe Y ( les autres membres de la structure sont identiques)
	diprg.diph.dwObj        = DIJOFS_Y; // axe à configuré (Y)
	// on affecte ces propriétés à l'interface
	if(pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph )!=DI_OK)
		return FALSE;
	

	DIPROPDWORD dipdw;	// structure qui contient la zone de neutralité sur l'axe X et Y
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    dipdw.diph.dwHow        = DIPH_BYOFFSET; 	// pour utiliser un format de donné pour dwObj	 
    dipdw.diph.dwObj        = DIJOFS_X; // axe à configuré (X)
	dipdw.dwData			=100;	// valeur à partir de laquelle les entrée sont détectés

	// affectation des propriétés à l'interface
	if(pJoystick->SetProperty( DIPROP_DEADZONE, &dipdw.diph )!=DI_OK)
		return FALSE;
	// idem pour l'axe Y
	dipdw.diph.dwObj        = DIJOFS_Y; // Specify the enumerated axis
	// affectation des propriétés à l'interface
	if(pJoystick->SetProperty( DIPROP_DEADZONE, &dipdw.diph )!=DI_OK)
		return FALSE;

    return TRUE;
}



// Destruction de al souris
void CJoystick::Destruction()
{
	if(pJoystick)pJoystick->Unacquire();
	SAFE_RELEASE(pJoystick);
}




// Mise à jour de l'états de  la souris
bool CJoystick::GetEtatJoystick(bool bActiveAquisition)
{
	HRESULT hr;		
	memset(&m_EtatJoyStick,0,sizeof(DIJOYSTATE2));	// ça évite les problèmes
  
    hr = pJoystick->Poll(); 
    if( FAILED(hr) )	// l'accès au joystick est perdu  
    {
        hr = pJoystick->Acquire();
        while( hr == DIERR_INPUTLOST ) // on attend que le joystick soit disponnible
            hr = pJoystick->Acquire();
    }

   // on récupère les donnés du joystick dans la structure
    if( FAILED( hr = pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_EtatJoyStick ) ) )
        pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_EtatJoyStick ); 


	return true;
}

