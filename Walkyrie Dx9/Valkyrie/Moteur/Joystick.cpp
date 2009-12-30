#include "Joystick.h"


CPeripherique* perif=NULL;
LPDIRECTINPUTDEVICE8 joy=NULL;


/******************* enum�ration des joystiques pr�sents **********************/
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,VOID* pContext )
{
    // on obtient une interface vers le joystick �num�r�.
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


// Cr�ation de la souris
bool CJoystick::Creation(LPDIRECTINPUT8 lpDI)
{
	// Appel d'un fonction callback pour �num�rer les joysticks install�s et branch�s
    if( lpDI->EnumDevices( DI8DEVCLASS_GAMECTRL, 
                                         EnumJoysticksCallback,
                                         NULL, DIEDFL_ATTACHEDONLY )!=DI_OK )
	{
		Console << "Creation CJoystick : EnumDevices �chou�" << endl;
        return FALSE;
	}
	pJoystick=joy;
	// aucain joystick install� et branch�
    if( NULL == pJoystick )
    {
		Console << "Creation CJoystick �chou� : aucun joystick branch�" << endl;
        return FALSE;
    }

	// d�finition du format de donn�
    if( pJoystick->SetDataFormat( &c_dfDIJoystick2 )!=DI_OK )
        return FALSE;

	// d�finition du niveau de coop�ration
	if( pJoystick->SetCooperativeLevel( m_pPeripherique->GetMoteur()->GetFenetre(), DISCL_EXCLUSIVE | DISCL_FOREGROUND )!=DI_OK)
        return FALSE;
	
	
	DIPROPRANGE diprg; // structure qui contient les propri�t�s de l'�tendue sur l'axe X et Y
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYOFFSET;	// pour utiliser un format de donn� pour dwObj	 
    diprg.diph.dwObj        = DIJOFS_X; // axe � configur� (X)
    diprg.lMin              = -12;	// �tendue sur l'axe X
    diprg.lMax              = +12;	// (nombre de points de contr�les)

	// on affecte ces propri�t�s � l'interface du joystick
	if(pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph )!=DI_OK)
		return FALSE;
	
	// configure l'axe Y ( les autres membres de la structure sont identiques)
	diprg.diph.dwObj        = DIJOFS_Y; // axe � configur� (Y)
	// on affecte ces propri�t�s � l'interface
	if(pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph )!=DI_OK)
		return FALSE;
	

	DIPROPDWORD dipdw;	// structure qui contient la zone de neutralit� sur l'axe X et Y
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    dipdw.diph.dwHow        = DIPH_BYOFFSET; 	// pour utiliser un format de donn� pour dwObj	 
    dipdw.diph.dwObj        = DIJOFS_X; // axe � configur� (X)
	dipdw.dwData			=100;	// valeur � partir de laquelle les entr�e sont d�tect�s

	// affectation des propri�t�s � l'interface
	if(pJoystick->SetProperty( DIPROP_DEADZONE, &dipdw.diph )!=DI_OK)
		return FALSE;
	// idem pour l'axe Y
	dipdw.diph.dwObj        = DIJOFS_Y; // Specify the enumerated axis
	// affectation des propri�t�s � l'interface
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




// Mise � jour de l'�tats de  la souris
bool CJoystick::GetEtatJoystick(bool bActiveAquisition)
{
	HRESULT hr;		
	memset(&m_EtatJoyStick,0,sizeof(DIJOYSTATE2));	// �a �vite les probl�mes
  
    hr = pJoystick->Poll(); 
    if( FAILED(hr) )	// l'acc�s au joystick est perdu  
    {
        hr = pJoystick->Acquire();
        while( hr == DIERR_INPUTLOST ) // on attend que le joystick soit disponnible
            hr = pJoystick->Acquire();
    }

   // on r�cup�re les donn�s du joystick dans la structure
    if( FAILED( hr = pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_EtatJoyStick ) ) )
        pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_EtatJoyStick ); 


	return true;
}

