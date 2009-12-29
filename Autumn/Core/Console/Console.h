#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <windows.h>
#include <iostream>

const char endl = '\n';

class CConsole
{
private:
	static HANDLE	m_hConsole;
	static int		m_NbInstance;

public:

	CConsole();
	~CConsole();

	void Printf(const char *szfmt, ...);
	void SetScreenBufferSize(short X, short Y);
	void SetCursorPosition(short X, short Y);
	int  GetNbInstance();

	CConsole& operator << (int i);
	CConsole& operator << (unsigned int u);
	CConsole& operator << (unsigned long u);
	CConsole& operator << (double f);
	CConsole& operator << (char* s);
	CConsole& operator << (const char* s);
	CConsole& operator << (char c);
	CConsole& operator << (RECT rc);
};

#if _DEBUG

static CConsole MyConsole;

#define LOG_INFO(arg1)								MyConsole << arg1
#define LOG_INFOLN(arg1)							MyConsole << arg1 << "\n"
#define LOG_PAUSE()									system( "pause" )

#else

#define LOG_INFO(arg1)								{void(0);}
#define LOG_INFOLN(arg1)							{void(0);}
#define LOG_PAUSE()									{void(0);}

#endif


#endif // _CONSOLE_H_



