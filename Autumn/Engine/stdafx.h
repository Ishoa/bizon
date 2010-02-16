// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#ifndef _WIN32_WINNT		// Autorise l'utilisation des fonctionnalités spécifiques à Windows XP ou version ultérieure.                   
#define _WIN32_WINNT 0x0501	// Attribuez la valeur appropriée à cet élément pour cibler d'autres versions de Windows.
#endif						

#define WIN32_LEAN_AND_MEAN		// Exclure les en-têtes Windows rarement utilisés

// 3D
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX11core.h>
#include <D3DX11async.h>
#include <D3DX11tex.h>
#include <D3Dcompiler.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef _UTILS_
#include "Core/Utils/Utils.h"
#endif

#ifndef _CONSOLE_H_
#include "Core/Console/Console.h"
#endif

//////////////////////////////////////////////////////////////////////////
// 3D
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

#ifndef _TERRAIN_TYPE_VERTEX_
#include "WrapperDX/Geometry/TerrainTypeVertex.h"
#endif

#ifndef _VERTEX_BUFFER_
#include "WrapperDX/Buffer/VertexBuffer.h"
#endif

#ifndef _VERTEX_BUFFER_EX_
#include "WrapperDX/Buffer/VertexBufferEx.h"
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

#ifndef _SAMPLER_ANISO_
#include "WrapperDX/Texture/Sampler/SamplerAniso.h"
#endif

#ifndef _SAMPLER_LINEAR_
#include "WrapperDX/Texture/Sampler/SamplerLinear.h"
#endif

#ifndef _SAMPLER_POINT_
#include "WrapperDX/Texture/Sampler/SamplerPoint.h"
#endif

#ifndef _SAMPLER_
#include "WrapperDX/Texture/Sampler/Sampler.h"
#endif

#ifndef _BLEND_OPERATION_ADD_
#include "WrapperDX/Device/State/BlendOperationAdd.h"
#endif

#ifndef _BLEND_OPERATION_SUB_
#include "WrapperDX/Device/State/BlendOperationSub.h"
#endif

#ifndef _BLEND_OPERATION_MIN_
#include "WrapperDX/Device/State/BlendOperationMin.h"
#endif

#ifndef _BLEND_OPERATION_MAX_
#include "WrapperDX/Device/State/BlendOperationMax.h"
#endif

#ifndef _BLEND_OPERATION_NONE_
#include "WrapperDX/Device/State/BlendOperationNone.h"
#endif

#ifndef _BLEND_STATE_
#include "WrapperDX/Device/State/BlendState.h"
#endif

//////////////////////////////////////////////////////////////////////////
// INPUT

#ifndef _INPUT_DEVICE_
#include "WrapperDX/InputDevice/InputDevice.h"
#endif

#ifndef _INPUT_CONTROLLER_
#include "WrapperDX/InputDevice/InputController.h"
#endif

#ifndef _KEYBOARD_
#include "WrapperDX/InputDevice/Keyboard.h"
#endif

#ifndef _MOUSE_
#include "WrapperDX/InputDevice/Mouse.h"
#endif

//////////////////////////////////////////////////////////////////////////
// MATH

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

#include "Core/TinyXML/tinystr.h"
#include "Core/TinyXML/tinyxml.h"