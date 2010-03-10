#include "stdafx.h"
#include "WindowApp/WindowApp.h"

// main
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	lpCmdLine;
	hPrevInstance;

	WindowApp * MyWindowApp = new WindowApp(1024, 768, false, "Autumn");
	if( FAILED( MyWindowApp->Create(hInstance, nCmdShow) ) )
	{
		LOG_PAUSE();
		return 0;
	}

	MyWindowApp->Loop();

	LOG_INFO( "Destruction de la fenetre : " );
	SAFE_DESTROY( MyWindowApp );

	_CrtDumpMemoryLeaks();

	return 0;

}

