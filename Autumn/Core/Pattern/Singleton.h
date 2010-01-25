#ifndef _SINGLETON_
#define _SINGLETON_

template< class T >
class Singleton
{
private:
	static T * ms_pInstance;

protected:
	Singleton< T > () {}
	~Singleton< T > () {}

public:

	static T *	CreateInstance()
	{
		if( ! ms_pInstance )
			ms_pInstance = new T;

		return ( static_cast< T * >(ms_pInstance) );
	}

	static void	DestroyInstance()
	{
		SAFE_DELETE( ms_pInstance );
	}

	static T *	GetInstance()
	{
		return ( static_cast< T * >(ms_pInstance) );
	}
};

template< class T > T * Singleton< T >::ms_pInstance = NULL;

#endif