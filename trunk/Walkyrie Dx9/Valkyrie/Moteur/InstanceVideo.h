#ifndef _CVIDEO_
#define _CVIDEO_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include <dshow.h>

class CInstanceVideo
{
private:

	IGraphBuilder *m_pGraphBuilder;
	IMediaControl *m_pMediaControl;
	IMediaEvent *m_pMediaEvent;
	IVideoWindow *m_pVideoWindow;
	IBasicAudio   *m_pBasicAudio;
	IBasicVideo   *m_pBasicVideo;
	IMediaSeeking *m_pMediaSeeking;

	long eventCode;

public:


	CInstanceVideo();
	~CInstanceVideo();

	bool InitInstanceVideo(BSTR strNomFichier);
	void InitFenetreVideo();
	void Run();
	void Stop();
	bool EstTermine();
	void Clean();
};

#endif