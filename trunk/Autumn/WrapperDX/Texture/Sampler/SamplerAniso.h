#ifndef _SAMPLER_ANISO_
#define _SAMPLER_ANISO_

class SamplerAniso
{
public:
	SamplerAniso();
	~SamplerAniso();

	static D3D11_SAMPLER_DESC GetSamplerDesc();
};

#endif // _SAMPLER_ANISO_