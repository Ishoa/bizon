#include "LecteurVideo.h"

CLecteurVideo::CLecteurVideo()
{


}

CLecteurVideo::~CLecteurVideo()
{


}

bool CLecteurVideo::InitLecteurVideo()
{
	if(FAILED(CoInitialize(NULL)))
		return false;

	if(FAILED(CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&m_pGraphBuilder)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&m_pMediaControl)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaEvent,
		(LPVOID *)&m_pMediaEvent)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IVideoWindow,
		(LPVOID *)&m_pVideoWindow)))
		return false;

	return true;
}

bool CLecteurVideo::ChargerVideo(BSTR strNomFichier)
{
	return SUCCEEDED(m_pMediaControl->RenderFile(strNomFichier));
		
}

void CLecteurVideo::Run()
{
	m_pMediaControl->Run();
}


void CLecteurVideo::Stop()
{
	m_pMediaControl->Stop();
}

void CLecteurVideo::Clean()
{
	SAFE_RELEASE(m_pGraphBuilder);
	SAFE_RELEASE(m_pMediaControl);
	SAFE_RELEASE(m_pMediaEvent);
	SAFE_RELEASE(m_pVideoWindow);

}