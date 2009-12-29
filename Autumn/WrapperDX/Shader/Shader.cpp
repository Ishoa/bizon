#include "stdafx.h"

#ifndef _SHADER_
#include "WrapperDX/Shader/Shader.h"
#endif

Shader::Shader()
: m_pBin(NULL)
{
	strcpy_s(m_filename, "no file");
}

Shader::Shader(const char * name)
: m_pBin(NULL)
{
	SetName(name);
}

Shader::~Shader()
{
}

void Shader::SetName(const char * name)
{
	strcpy_s(m_filename, name);
}

HRESULT Shader::CompileAndCreate()
{
	D_RETURN( Compile() );

	D_RETURN( Create() );

	return S_OK;
}

HRESULT Shader::Compile()
{
	char strFile[64];

	strcpy_s(strFile, SHADER_PATH);
	strcat_s(strFile, m_filename);

	ID3DBlob * pErrors = NULL;

	if( FAILED( D3DX11CompileFromFile(strFile, NULL /*pDefines*/, NULL/* pInclude */, GetEntryPoint(), GetShaderVersion(), SHADER_FLAG, NULL, NULL/*pThreadPump*/, &m_pBin, &pErrors, NULL) ) )
	{
		LOG_INFO( strFile );
		if( ! pErrors )
			LOG_INFOLN( " :Fichier introuvable" );
		else
			LOG_INFOLN( " : Failed\n" << (LPCSTR)( pErrors->GetBufferPointer() ) );
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Shader::Destroy()
{
	SAFE_RELEASE(m_pBin);

	return S_OK;
}