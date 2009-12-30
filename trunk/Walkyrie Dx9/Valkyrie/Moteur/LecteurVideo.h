#ifndef _CVIDEO_
#define _CVIDEO_

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"
#include <dshow.h>

class CLecteurVideo
{
private:
	IGraphBuilder *m_pGraphBuilder;
	IMediaControl *m_pMediaControl;
	IMediaEvent *m_pMediaEvent;
	IVideoWindow *m_pVideoWindow;

	long eventCode;

public:


	CLecteurVideo();
	~CLecteurVideo();

	bool ChargerVideo(BSTR strNomFichier);

	bool InitLecteurVideo();
	void Run();
	void Stop();
	void Clean();
};

#endif