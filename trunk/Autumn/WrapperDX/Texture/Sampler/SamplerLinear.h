#ifndef _SAMPLER_LINEAR_
#define _SAMPLER_LINEAR_

class SamplerLinear
{
public:
	SamplerLinear();
	~SamplerLinear();

	static D3D11_SAMPLER_DESC GetSamplerDesc();
};

#endif // _SAMPLER_LINEAR_