#include "stdafx.h"

#ifndef _SAMPLER_POINT_
#include "WrapperDX/Texture/Sampler/SamplerPoint.h"
#endif

SamplerPoint::SamplerPoint()
{

}

SamplerPoint::~SamplerPoint()
{

}

D3D11_SAMPLER_DESC SamplerPoint::GetSamplerDesc()
{
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory( &SamplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return SamplerDesc;
}