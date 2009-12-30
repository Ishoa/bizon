
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


#include "VariableGlobale.h"
#include "VariableGlobalDX.h"


#include "Moteur.h"
#include "..\Scenes\MyScenario.h"

#include "resource.h"

#define LARGEURFENETRE 1024
#define HAUTEURFENETRE 768


CConsole Console;
CDXDataBase DxDataBase;

static HWND ChoixDialog;

bool g_bchoix;
bool g_bScenario;
bool g_bFinish;

int g_fHauteur = HAUTEURFENETRE;
int g_fLargeur = LARGEURFENETRE;
bool g_bPleinEcran = false;

BOOL CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Select;
	CScene* pScene = NULL;
	CMyScenario* pScenario = new  CMyScenario();
	CMyScenario::EScene eScene = CMyScenario::e_Scene_Test;

	switch(msg)
	{
		case WM_CLOSE:
			g_bchoix = true;
			break;

		case WM_COMMAND:
			Select=LOWORD(wParam);
			switch(Select)
			{
				case IDOK:

						EndDialog(hWnd,0);
						
						if(IsDlgButtonChecked(hWnd,IDC_Shadow) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_ShadowVolume;

						if(IsDlgButtonChecked(hWnd,IDC_SHADER) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_Shader;

						if(IsDlgButtonChecked(hWnd,IDC_TEST) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_Test;

						if(IsDlgButtonChecked(hWnd,IDC_NORMAL_MAPPING) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_NormalMapping;

						if(IsDlgButtonChecked(hWnd,IDC_Laby) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_Laby;

						if(IsDlgButtonChecked(hWnd,IDC_RADIO_MIPMAP) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_MipMapping;

						if(IsDlgButtonChecked(hWnd,IDC_SYSTEMSOLAIRE) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_SystemeSolaire;

						if(IsDlgButtonChecked(hWnd,IDC_RADIO_BASIQUE) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_Basique;

						if(IsDlgButtonChecked(hWnd,IDC_MULTIPLEVERTEXBUFFERS) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_MultipleVertexBuffers;

						if(IsDlgButtonChecked(hWnd,IDC_RADIO_SURVOL) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_Terrain3D;

						if(IsDlgButtonChecked(hWnd,IDC_TEAPOT) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_TeapotAndTracball;

						if(IsDlgButtonChecked(hWnd,IDC_PARTICULE) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_ParticleSystem;

						if(IsDlgButtonChecked(hWnd,IDC_EFFECTDX) == BST_CHECKED)
							eScene=CMyScenario::e_Scene_EffectDX;

						if(pScenario == NULL || !CMoteur::GetCurrent()->InitScenario(pScenario))
						{
							MessageBox(NULL, "Erreur à l'attribution de la scene", "Erreur ", MB_OK);
							g_bScenario =false;
						}
						else
						{
							pScenario->SetScene(eScene);
							pScenario->Reset();
							g_bScenario =true;
						}
						g_bchoix = true;
					return TRUE;
					break;

				case IDCANCEL:
					g_bchoix = true;
					g_bFinish = true;
					CMoteur::GetCurrent()->Destruction();
					DestroyWindow(CMoteur::GetCurrent()->GetFenetre());
					EndDialog(hWnd,Select);
					PostQuitMessage(0);
					return TRUE;
					break;
			}
			break;

		default:
			return FALSE;
	}

	return false;
}

// Point d'entrée du programme 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CMoteur* pMoteur = CMoteur::GetCurrent();

	g_bchoix = false;
	g_bScenario = false;
	g_bFinish = false;

	bool bReturn;

	do
	{
		// Création du moteur
		if(!pMoteur->Creation(hInstance,g_fLargeur,g_fHauteur,g_bPleinEcran))
		{
			MessageBox(NULL, "Erreur à la création de l'application", "Erreur", MB_OK);
			return -1;
		}

		g_bchoix = false;
		g_bScenario = false;
		HWND test = pMoteur->GetFenetre();

		ChoixDialog = CreateDialog(hInstance, (LPCTSTR)IDD_DIALOG_STARTER, test, (DLGPROC)DlgProc);	

		// On affiche la fenêtre
		ShowWindow(ChoixDialog, SW_SHOW);
		CheckDlgButton(ChoixDialog,IDC_RADIO_BASIQUE,BST_CHECKED);

		MSG msg;
		// Initialisation de la variable des Messages
		ZeroMemory(&msg, sizeof(msg));

		// Boucle de messages principale
		while(!g_bchoix)
		{
			GetMessage(&msg,ChoixDialog,0,0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		// Si la création c'est bien passé on lance le moteur
		bReturn =  g_bScenario && pMoteur->Demarrer();

		CMoteur::GetCurrent()->Destruction();
		DxDataBase.ReleaseDataBase();
		DxDataBase.affichageStatus();
	}
	while(!g_bFinish);

	_CrtDumpMemoryLeaks();

	return bReturn;

}
