#ifndef _FILE_
#define _FILE_

#include <stdio.h>

class File
{
public:
	enum eTypeOpen
	{
		eTypeRead,
		eTypeWrite,
		eTypeNotOpen
	};
	enum eTypeFile
	{
		eTypeText,
		eTypeBinary
	};

private:
	FILE		* m_pFile;
	char		m_sName[256];
	eTypeFile	m_eTypeFile;
	eTypeOpen	m_eTypeOpen;

public:

	File();
	File(const char * _name);
	~File();

	void SetName(const char * _name);

	bool Open(eTypeOpen _eTypeOpen, eTypeFile _eTypeFile = eTypeText);
	bool Close();

	bool Write(int _iValue);
	bool Write(float _fValue);
	bool Write(char _cValue);
	bool Write(const char * _sValue);

	bool Read(int & _iValue);
	bool Read(float & _fValue);
	bool Read(char & _cValue);
	bool Read(char * _sValue, unsigned int _iSize);

	inline FILE * GetFile() const { return m_pFile; }

};

#endif