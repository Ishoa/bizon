#pragma once
#include "VariableGlobale.h"

class CFenetreWin32
{
public:
	CFenetreWin32(void);
	~CFenetreWin32(void);
	bool Creation(HINSTANCE hInstance, int Largeur = 100, int Hauteur = 100, bool PleinEcran = false);
	LRESULT  MgsProcess( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProcess(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);
	DWORD m_dwWindowStyle;
	char m_szTitle[32];
	char m_szWindowClass[32];
	HWND m_hWnd;
	bool m_bActive;
};
