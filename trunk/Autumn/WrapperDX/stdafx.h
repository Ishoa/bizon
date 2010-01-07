// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s



// TODO�: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
#ifndef _UTILS_
#include "Core/Utils/Utils.h"
#endif

#ifndef _CONSOLE_H_
#include "Core/Console/Console.h"
#endif

#include "Core/TinyXML/tinystr.h"
#include "Core/TinyXML/tinyxml.h"

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

// DX11
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX11tex.h>
#include <D3Dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")




#ifndef _SINGLETON_
#include "Core/Pattern/Singleton.h"
#endif

#ifndef _VECTOR1_
#include "Core/Math/Vector1.h"
#endif

#ifndef _VECTOR2_
#include "Core/Math/Vector2.h"
#endif

#ifndef _VECTOR3_
#include "Core/Math/Vector3.h"
#endif

#ifndef _VECTOR4_
#include "Core/Math/Vector4.h"
#endif

#ifndef _MATRIX4X4_
#include "Core/Math/Matrix4x4.h"
#endif

#ifndef _SPHERE_
#include "Core/Math/Sphere.h"
#endif

#ifndef _COLORD_
#include "Core/Math/Color.h"
#endif
