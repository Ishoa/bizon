#include "stdafx.h"

#ifndef _SAMPLER_ANISO_
#include "WrapperDX/Texture/Sampler/SamplerAniso.h"
#endif

SamplerAniso::SamplerAniso()
{

}

SamplerAniso::~SamplerAniso()
{

}

D3D11_SAMPLER_DESC SamplerAniso::GetSamplerDesc()
{
	D3D11_SAMPLER_DESC SamplerDesc;
	ZeroMemory( &SamplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return SamplerDesc;
}