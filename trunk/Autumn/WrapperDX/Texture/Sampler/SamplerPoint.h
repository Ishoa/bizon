#ifndef _SAMPLER_POINT_
#define _SAMPLER_POINT_

class SamplerPoint
{
public:
	SamplerPoint();
	~SamplerPoint();

	static D3D11_SAMPLER_DESC GetSamplerDesc();
};

#endif // _SAMPLER_POINT_