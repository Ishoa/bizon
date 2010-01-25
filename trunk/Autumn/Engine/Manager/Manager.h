#ifndef _MANAGER_
#define _MANAGER_

class Manager
{
protected:

public:

	enum eManager
	{
		eManager_TIME = 0,
		eManager_INPUT,
		eManager_COUNT,
	};


	Manager();
	virtual ~Manager();

	virtual HRESULT Create() = 0;
	virtual HRESULT Update() = 0;
	virtual HRESULT Destroy() = 0;

};

#endif // _MANAGER_