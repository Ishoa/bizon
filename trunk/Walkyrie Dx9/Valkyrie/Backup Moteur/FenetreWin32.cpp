#include "FenetreWin32.h"


CFenetreWin32::CFenetreWin32(void)
{
	m_hWnd = NULL;
	m_dwWindowStyle = NULL;

	m_bActive = true;

	sprintf(m_szTitle, "Pre Alpha Engine");
	sprintf(m_szWindowClass, "3D Windows");
}

CFenetreWin32::~CFenetreWin32(void)
{
}

LRESULT CALLBACK CFenetreWin32::WndProcess(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
	return 0;//Moteur.FenetrePrincipale.MgsProcess(hWnd, uMsg, wParam, lParam);
}

LRESULT CFenetreWin32::MgsProcess( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	RECT rc = {0, 0, 150, 150};

	switch( uMsg )
    {
        case WM_PAINT:
            MessageBeep(-1);
            break;

		case WM_ACTIVATE:
			if (wParam != WA_INACTIVE)
			{
				m_bActive = true;
			}
			else
			{
				m_bActive = false;
			}
			break;

		case WM_GETMINMAXINFO:
			AdjustWindowRect(&rc, m_dwWindowStyle, false);
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = rc.right-rc.left;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = rc.bottom-rc.top;
            break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            m_hWnd = NULL;
            return 0;
    }
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

bool CFenetreWin32::Creation(HINSTANCE hInstance, int Largeur, int Hauteur, bool PleinEcran)
{
	// Icône de l'application
	HICON hIcone = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON));

	// Paramètre de la classe windows
	WNDCLASS wndClass;

	wndClass.style         = NULL;						// Style de la fen^étre
    wndClass.lpfnWndProc   = WndProcess;		// Pointeur sur la fonction qui traite les méssages
    wndClass.cbClsExtra    = 0;							// Octect supplémentaire disponible pour la classe
    wndClass.cbWndExtra    = 0;							// Octect supplémentaire disponible pour la fenêtre
    wndClass.hInstance     = hInstance;					// Handle d'instance de l'application
	wndClass.hIcon         = hIcone;					// Icone de le fenêtre
    wndClass.hCursor       = LoadCursor(0, IDC_ARROW);	// Curseur de la fenêtre
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);	// Couleur de fond
    wndClass.lpszMenuName  = NULL;						// Nom du menu
    wndClass.lpszClassName = m_szWindowClass;				// Nom de la classe

    // Enregistre la classe
	if (!RegisterClass( &wndClass ))						
	{
		Console << "Ce programme requière Win32!" << endl;
		return false;
	}

	// Style de la fenêtre
	m_dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

	// Taille la fenêtre
	RECT rc = {0,0,200,200};
	AdjustWindowRect( &rc, m_dwWindowStyle, false );

	// Création de la fenêtre
	m_hWnd = CreateWindow
		(
			m_szWindowClass,		// Nom de la classe enregistrée
			m_szTitle,				// Nom de la fenêtre
			m_dwWindowStyle,		// Style de la fenêtre
			CW_USEDEFAULT,			// Position horizontale de la fenêtre
			CW_USEDEFAULT,			// Position verticale de la fenêtre
			rc.right-rc.left,		// Largeur de la fenêtre
			rc.bottom-rc.top,		// Hauteur de la fenêtre
			NULL,					// Gestionnaire de la fenêtre parente
			NULL,					// Gestionnaire de menu
			hInstance,				// Gestionnaire de l'instance d'application
			NULL					// Paramètre de création de la fenêtre
		);

	// On vérifie si la fenêtre est bien crée
	if(!m_hWnd)
	{
		Console << "Impossible de créer la fenêtre!" << endl;
		return false;
	}

	return true;
}


