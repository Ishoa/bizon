// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalit�s sp�cifiques � Windows�XP ou version ult�rieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropri�e � cet �l�ment pour cibler d'autres versions de Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX11tex.h>
#include <D3Dcompiler.h>

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

#ifndef _DEPTH_STENCIL_
#include "WrapperDX/Device/DepthStencil.h"
#endif

#ifndef _UTILS_3D_
#include "WrapperDX/Utils/Utils.h"
#endif

#ifndef _TYPE_VERTEX_
#include "WrapperDX/Geometry/TypeVertex.h"
#endif

#ifndef _DISPLAY_OBJECT_TYPE_VERTEX_
#include "WrapperDX/Geometry/DisplayObjectTypeVertex.h"
#endif

#ifndef _VERTEXBUFFER_
#include "WrapperDX/Buffer/VertexBuffer.h"
#endif

#ifndef _INDEXBUFFER_
#include "WrapperDX/Buffer/IndexBuffer.h"
#endif

#ifndef _VERTEX_LAYOUT_
#include "WrapperDX/Buffer/VertexLayout.h"
#endif

#ifndef _VERTEX_SHADER_
#include "WrapperDX/Shader/VertexShader.h"
#endif

#ifndef _PIXEL_SHADER_
#include "WrapperDX/Shader/PixelShader.h"
#endif

#ifndef _CONSTANT_BUFFER_
#include "WrapperDX/Buffer/ConstantBuffer.h"
#endif

#ifndef _CONSTANT_BUFFER_UPDATE_
#include "WrapperDX/Buffer/ConstantBufferUpdate.h"
#endif

#ifndef _TEXTURE_
#include "WrapperDX/Texture/Texture.h"
#endif

#ifndef _TEXTURE2D_
#include "WrapperDX/Texture/Texture2D.h"
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

