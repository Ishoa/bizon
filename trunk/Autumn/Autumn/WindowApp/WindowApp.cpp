#include "stdafx.h"
#include "WindowApp.h"

WindowApp * WindowApp::ms_pInstance = NULL;

// Constructeur / Destructeur
WindowApp::WindowApp(int width, int height, bool bFullscreen , char * title)
: m_uWidth(width)
, m_uHeight(height)
, m_bFullscreen(bFullscreen)
{
	strcpy_s(m_sTitle, title);
	strcpy_s(m_sClassName, "Windows Application");
}

WindowApp::~WindowApp()
{

}

// Fonctions Public
HRESULT WindowApp::Create(HINSTANCE hInstance, int nCmdShow)
{
	if( ms_pInstance )
	{
		LOG_INFOLN("Error : Instance already created");
		return E_FAIL;
	}

	ms_pInstance = this;

	// Register the class
	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = NULL;//LoadIcon(NULL, "..\\Data\\Icones\\Nuts.ico");
	wcex.hCursor        = NULL;
	wcex.hbrBackground  = NULL;
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = m_sClassName;
	wcex.hIconSm        = NULL;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, m_uWidth, m_uHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
	m_hWnd = CreateWindow( m_sClassName, m_sTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if( !m_hWnd )
		return E_FAIL;

	m_pEngine = new Viewer;
	D_RETURN( m_pEngine->Create(m_hWnd, m_uWidth, m_uHeight, m_bFullscreen) );

	ShowWindow( m_hWnd, nCmdShow );
	UpdateWindow( m_hWnd );

	return S_OK;
}

HRESULT WindowApp::Destroy()
{
	SAFE_DESTROY( m_pEngine );

	return S_OK;
}

void WindowApp::Loop()
{
	MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			m_pEngine->Update();
		}
    }
}

// Fonctions Protected
LRESULT CALLBACK WindowApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int xPos = 0;
	int yPos = 0;
	int uSizeX = 0;
	int uSizeY = 0;
	unsigned int key  = 0;
	switch(msg) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_SIZE:
			uSizeX = GET_X_LPARAM(lParam);
			uSizeY = GET_Y_LPARAM(lParam);
			LOG_INFOLN("Resize : " << uSizeX << " x " << uSizeY);
			WindowApp::GetInstance()->GetEngine()->Resize((unsigned int)uSizeX, (unsigned int)uSizeY);
			break;

//////////////////////////////////////////////////////////////////////////
// MOUSE
		case WM_MOUSEMOVE:
			xPos = GET_X_LPARAM(lParam); 
			yPos = GET_Y_LPARAM(lParam);
			WindowApp::GetInstance()->GetEngine()->UpdateMouse(xPos,yPos);
			break;

		// Left Button
		case WM_LBUTTONDOWN:
			WindowApp::GetInstance()->GetEngine()->OnLButtonDown();
			break;

		case WM_LBUTTONUP:
			WindowApp::GetInstance()->GetEngine()->OnLButtonUp();
			break;

		// Right Button
		case WM_RBUTTONDOWN:
			WindowApp::GetInstance()->GetEngine()->OnRButtonDown();
			break;

		case WM_RBUTTONUP:
			WindowApp::GetInstance()->GetEngine()->OnRButtonUp();
			break;

		// Middle Button
		case WM_MBUTTONDOWN:
			WindowApp::GetInstance()->GetEngine()->OnMButtonDown();
			break;

		case WM_MBUTTONUP:
			WindowApp::GetInstance()->GetEngine()->OnMButtonUp();
			break;
 
		case WM_MOUSEWHEEL:
			break;


//////////////////////////////////////////////////////////////////////////
// KEYBOARD
		case WM_KEYDOWN:
			key = (int)wParam;
			WindowApp::GetInstance()->GetEngine()->OnKeyPressed(key);
			break;

		case WM_KEYUP:
			key = (int)wParam;
			WindowApp::GetInstance()->GetEngine()->OnKeyReleased(key);
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
