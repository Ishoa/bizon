#ifndef _SAMPLER_
#define _SAMPLER_

class SamplerBase
{
public:
	SamplerBase(){}
	virtual ~SamplerBase(){}

	virtual ID3D11SamplerState * GetSampler() const { return NULL; }
};

template < class Samplertype >
class Sampler : public SamplerBase
{
private:
	ID3D11SamplerState	* m_pSampler;
public:
	Sampler();
	~Sampler();

	HRESULT Create();
	HRESULT Destroy();

	virtual ID3D11SamplerState * GetSampler() const { return m_pSampler; }
};

template < class Samplertype >
Sampler< Samplertype >::Sampler()
: m_pSampler(NULL)
{

}

template < class Samplertype >
Sampler< Samplertype >::~Sampler()
{

}

template < class Samplertype >
HRESULT Sampler< Samplertype >::Create()
{
	D3D11_SAMPLER_DESC SamplerDesc = Samplertype::GetSamplerDesc();
	D_RETURN( g_pDxDevice->CreateSamplerState( &SamplerDesc, &m_pSampler ) );
	return S_OK;
}

template < class Samplertype >
HRESULT Sampler< Samplertype >::Destroy()
{
	SAFE_RELEASE( m_pSampler );
	return S_OK;
}


#endif // _SAMPLER_