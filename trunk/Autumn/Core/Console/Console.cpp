#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "Console.h"

int    CConsole::m_NbInstance = 0;
HANDLE CConsole::m_hConsole = NULL;

CConsole::CConsole()
{
	m_NbInstance++;
	if(m_NbInstance == 1)
	{
		AllocConsole();
		SetConsoleTitle("Debug Console");
		SetScreenBufferSize(80, 100);
		SetCursorPosition(0, 0);
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
}

CConsole::~CConsole()
{
	m_NbInstance--;
    if(m_NbInstance == 0)         
    {        
        FreeConsole();        
        CloseHandle(m_hConsole);  
		m_hConsole = NULL;
    }
}

void CConsole::Printf(const char *szfmt, ...)
{
	char s[300];
    va_list argptr;
    int cnt;
    
    va_start(argptr, szfmt);
    cnt = vsprintf_s(s, szfmt, argptr);
    va_end(argptr);
    
    DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
}

void CConsole::SetScreenBufferSize(short X, short Y)
{
	if(m_hConsole)
	{
		COORD co = {X, Y };
		SetConsoleScreenBufferSize(m_hConsole, co);
	}
}

void CConsole::SetCursorPosition(short X, short Y)
{
	if(m_hConsole)
	{
		COORD co = {X, Y };
		SetConsoleCursorPosition(m_hConsole,co);
	}
}

int  CConsole::GetNbInstance()
{
	return m_NbInstance;
}

CConsole& CConsole::operator << (int i)
{
	char s[50];
	sprintf_s(s, "%d", i);

	DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (unsigned int u)
{
	char s[50];
	sprintf_s(s, "%d", u);

	DWORD cCharsWritten;
	if(m_hConsole)
	{
		WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
	}
	return *this;
}

CConsole& CConsole::operator << (unsigned long u)
{
	char s[50];
	sprintf_s(s, "%d", u);

	DWORD cCharsWritten;
	if(m_hConsole)
	{
		WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
	}
	return *this;
}

CConsole& CConsole::operator << (double f)
{
	char s[50];
	sprintf_s(s, "%f", f);

	DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (char* s)
{
	DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (const char* s)
{
	DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (char c)
{
	char s[5];
	sprintf_s(s, "%c", c);

	DWORD cCharsWritten;
    if(m_hConsole)
    {
        WriteConsole(m_hConsole, s, (DWORD)strlen(s), &cCharsWritten, NULL);
    }
	return *this;
}

CConsole& CConsole::operator << (RECT rc)
{
	*this << endl;
	*this << "RECT.left = "   << rc.left   << endl;
	*this << "RECT.top = "    << rc.top    << endl;
	*this << "RECT.right = "  << rc.right  << endl;
	*this << "RECT.bottom = " << rc.bottom << endl;
	*this << endl;
	return *this;
}
