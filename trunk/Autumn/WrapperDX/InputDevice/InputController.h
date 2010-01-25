#ifndef _INPUT_CONTROLLER_
#define _INPUT_CONTROLLER_

struct IDirectInputDevice8;

class InputController
{
protected:
	IDirectInputDevice8	* m_pInputController;

public:
	InputController();
	virtual ~InputController();

	virtual HRESULT Create()=0;
	virtual HRESULT Destroy();
	virtual HRESULT Poll()=0;
	
	inline bool IsInitialized() const { return ( m_pInputController != NULL ); };
};

#endif // _INPUT_CONTROLLER_