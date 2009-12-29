#ifndef _WINDOWAPP_
#define _WINDOWAPP_

#include <windows.h>

class Engine;


class WindowApp
{
protected:

	static WindowApp		* ms_pInstance;

	unsigned int			m_uWidth;
	unsigned int			m_uHeight;
	bool					m_bFullscreen;
	char					m_sTitle[64];
	char					m_sClassName[64];
	HWND					m_hWnd;

	Engine					* m_pEngine;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	WindowApp(int width = 800, int height = 600, bool bFullscreen = false, char * title = "Windows Application");
	~WindowApp();

	
	static WindowApp	* GetInstance() { return ms_pInstance; }
	
	Engine		* GetEngine() { return m_pEngine; }
	
	HRESULT Create(HINSTANCE hInstance, int nCmdShow);
	HRESULT Destroy();
	void	Loop();

};

#endif // _WINDOWAPP_


