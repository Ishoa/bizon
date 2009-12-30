#include "InstanceVideo.h"

CInstanceVideo::CInstanceVideo()
{
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pMediaEvent = NULL;
	m_pVideoWindow = NULL;
	m_pBasicAudio = NULL;
	m_pBasicVideo = NULL;
	m_pMediaSeeking = NULL;


}

CInstanceVideo::~CInstanceVideo()
{


}

bool CInstanceVideo::InitInstanceVideo(BSTR strNomFichier)
{

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

	if(FAILED(m_pMediaControl->RenderFile(strNomFichier)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaSeeking, (void **)&m_pMediaSeeking)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void **)&m_pBasicVideo)))
		return false;

	if(FAILED(m_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&m_pBasicAudio)))
		return false;
/*


	float fLargeur,fLongeur;
	m_pBasicVideo->GetVideoSize(&fLargeur, &fLongeur));

	fLargeur=CMoteur::GetCurrent()->GetLargeurFenetre();
	fLongeur=CMoteur::GetCurrent()->GetHauteurFenetre();
*/


	return true;
}


void CInstanceVideo::Run()
{
	if(m_pMediaControl)
		m_pMediaControl->Run();
}


bool CInstanceVideo::EstTermine()
{
	long lEventCode;
	long lParam1;
	long lParam2;
	m_pMediaEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
	if (EC_COMPLETE == lEventCode) {
		m_pMediaEvent->FreeEventParams(lEventCode, lParam1, lParam2);
		return(false);
	}
	return(true);
}

void CInstanceVideo::Stop()
{
	m_pMediaControl->Stop();
}

void CInstanceVideo::Clean()
{
	SAFE_RELEASE(m_pGraphBuilder);
	SAFE_RELEASE(m_pMediaControl);
	SAFE_RELEASE(m_pMediaEvent);
	SAFE_RELEASE(m_pVideoWindow);
	SAFE_RELEASE(m_pBasicAudio);
	SAFE_RELEASE(m_pBasicVideo);
	SAFE_RELEASE(m_pMediaSeeking);
}