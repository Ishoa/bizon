// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclure les en-têtes Windows rarement utilisés



// TODO : faites référence ici aux en-têtes supplémentaires nécessaires au programme

#ifndef _UTILS_
#include "Core/Utils/Utils.h"
#endif

#ifndef _CONSOLE_H_
#include "Core/Console/Console.h"
#endif

#ifndef _VIEW_PORT_
#include "WrapperDX/Device/ViewPort.h"
#endif

#ifndef _DEVICE3D_
#include "WrapperDX/Device/Device.h"
#endif

#ifndef _RENDER_TARGET_
#include "WrapperDX/Device/RenderTarget.h"
#endif

#ifndef _UTILS_3D_
#include "WrapperDX/Utils/Utils.h"
#endif

#ifndef _DISPLAY_OBJECT_
#include "Engine/SceneGraph/DisplayObject.h"
#endif

#ifndef _PLAN_
#include "Engine/SceneGraph/Plan.h"
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
