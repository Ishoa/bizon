#ifndef _UTILS_
#define _UTILS_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////
#define WM_MOUSEWHEEL 0x020A
#define WHEEL_DELTA   120

//////////////////////////////////////////////////////////////////////////
#define UNREF_PARAM( x )	{ x; }

//////////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE( x )		{ if( x != NULL ) { ( x )->Release();			( x ) = NULL; } }
#define SAFE_DELETE( x )		{ if( x != NULL ) {					  delete x;	( x ) = NULL; } }
#define SAFE_DELETE_ARRAY( x )	{ if( x != NULL ) {				   delete [] x;	( x ) = NULL; } }
#define SAFE_DESTROY( x )		{ if( x != NULL ) { ( x )->Destroy(); delete x; ( x ) = NULL; } }

//////////////////////////////////////////////////////////////////////////
#define F_RETURN( fFunction, strArg )	{ HRESULT hResult; LOG_INFOLN(strArg); if( FAILED( hResult = fFunction ) ) { return hResult; } }
#define E_RETURN( fFunction, strArg )	{ HRESULT hResult; LOG_INFO(strArg); if( FAILED( hResult = fFunction ) ) { LOG_INFOLN("FAILED"); return hResult; } LOG_INFOLN("OK"); }
#define D_RETURN( fFunction )			{ HRESULT hResult; if( FAILED( hResult = fFunction ) ) { return hResult; }  }

//////////////////////////////////////////////////////////////////////////
#ifndef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport)
#endif

//////////////////////////////////////////////////////////////////////////
// Shaders
#define TERRAIN_PATH			"..\\Data\\Terrain\\"
#define TEXTURE_PATH			"..\\Data\\Textures\\"
#define OBJECT_PATH				"..\\Data\\Objects\\"
#define OBJECT_PLY_PATH			"..\\Data\\Objects\\PLY\\"
#define SHADER_PATH				"..\\Data\\Shaders\\"
#define EFFECT_PATH				"..\\Data\\Effects\\"
#define EFFECT_POOL_PATH		"..\\Data\\Effects\\Pool\\"

#ifdef _DEBUG
#define SHADER_FLAG		D3D10_SHADER_DEBUG
#else
#define SHADER_FLAG		NULL
#endif

#define KEYBOARD_COOPLEVEL		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
#define MOUSE_COOPLEVEL			DISCL_FOREGROUND | DISCL_NONEXCLUSIVE

#endif