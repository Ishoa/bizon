#ifndef _BLEND_STATE_
#define _BLEND_STATE_

template < class Operationtype >
class BlendState
{
private:
	ID3D11BlendState	* m_pBlendState;
public:
	BlendState();
	~BlendState();

	HRESULT Create();
	HRESULT Destroy();

	inline ID3D11BlendState * GetBlendState() const { return m_pBlendState; }

};


template < class Operationtype >
BlendState< Operationtype >::BlendState()
: m_pBlendState( NULL )
{

}

template < class Operationtype >
BlendState< Operationtype >::~BlendState()
{

}

template < class Operationtype >
HRESULT BlendState< Operationtype >::Create()
{
	D3D11_BLEND_DESC BlendDesc;
	BlendDesc.AlphaToCoverageEnable = Operationtype::IsEnable();
	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable			= Operationtype::IsEnable();
	BlendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend				= ( Operationtype::IsEnable() ? D3D11_BLEND_ONE : D3D11_BLEND_ZERO );
	BlendDesc.RenderTarget[0].BlendOp				= Operationtype::GetBlendOperation();
	BlendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha		= ( Operationtype::IsEnable() ? D3D11_BLEND_ONE : D3D11_BLEND_ZERO );
	BlendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;
	D_RETURN( g_pDxDevice->CreateBlendState( &BlendDesc, &m_pBlendState ) );

	return S_OK;
}

template < class Operationtype >
HRESULT BlendState< Operationtype >::Destroy()
{
	SAFE_RELEASE( m_pBlendState );
	return S_OK;
}

#endif // _BLEND_STATE_