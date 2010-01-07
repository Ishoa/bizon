#include "stdafx.h"

#ifndef _FILE_
#include "Core/File/File.h"
#endif

File::File()
: m_pFile(NULL)
, m_eTypeOpen(eTypeNotOpen)
, m_eTypeFile(eTypeText)
{
}

File::File(const char * _name)
{
	strcpy_s(m_sName, _name);
}

File::~File()
{
}

void File::SetName(const char * _name)
{
	strcpy_s(m_sName, _name);
}

bool File::Open(eTypeOpen _eTypeOpen, eTypeFile _eTypeFile)
{
	char sType[3];

	m_eTypeOpen = _eTypeOpen;
	m_eTypeFile = _eTypeFile;


	if( ! m_sName )
		return false;

	switch ( _eTypeOpen )
	{
	case eTypeNotOpen : return false;   break;
	case eTypeRead    : sType[0] = 'r'; break;
	case eTypeWrite   : sType[0] = 'w'; break;
	default : return false; break;
	}

	switch ( _eTypeFile )
	{
	case eTypeText   : sType[1] = 't'; break;
	case eTypeBinary : sType[1] = 'b'; break;
	default : return false; break;
	}

	sType[2] = '\0';

	 int ret = fopen_s(&m_pFile, m_sName, sType);
	 if( ret == 0 )
		 return true;
	 else
		 return false;
}

bool File::Close()
{
	fclose(m_pFile);
	return true;
}

bool File::Write(int _iValue)
{
	if( m_eTypeOpen == eTypeRead )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fwrite(&_iValue, sizeof(int), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fprintf_s(m_pFile, "%d", _iValue);
	else
		return false;

	return true;
}

bool File::Write(float _fValue)
{
	if( m_eTypeOpen == eTypeRead )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fwrite(&_fValue, sizeof(float), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fprintf_s(m_pFile, "%f", _fValue);
	else
		return false;

	return true;
}

bool File::Write(char _cValue)
{
	if( m_eTypeOpen == eTypeRead )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fwrite(&_cValue, sizeof(char), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fprintf_s(m_pFile, "%c", _cValue);
	else
		return false;

	return true;
}

bool File::Write(const char * _sValue)
{
	if( m_eTypeOpen == eTypeRead )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fwrite(_sValue, sizeof(char), strlen( _sValue ), m_pFile);
	else if( m_eTypeFile == eTypeText )
		fprintf_s(m_pFile, "%s", _sValue);
	else
		return false;

	return true;
}

bool File::Read( int & _iValue )
{
	if( m_eTypeOpen == eTypeWrite )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fread(&_iValue, sizeof(int), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fscanf_s(m_pFile, "%d", &_iValue);
	else
		return false;

	return true;
}

bool File::Read( float & _fValue )
{
	if( m_eTypeOpen == eTypeWrite )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fread(&_fValue, sizeof(float), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fscanf_s(m_pFile, "%f", &_fValue);
	else
		return false;

	return true;
}

bool File::Read( char & _cValue )
{
	if( m_eTypeOpen == eTypeWrite )
		return false;

	if( m_eTypeFile == eTypeBinary )
		fread(&_cValue, sizeof(char), 1, m_pFile);
	else if( m_eTypeFile == eTypeText )
		fscanf_s(m_pFile, "%c", &_cValue);
	else
		return false;

	return true;
}

bool File::Read( char * _sValue, unsigned int _iSize )
{
	if( m_eTypeOpen == eTypeWrite )
		return false;

	if( m_eTypeFile == eTypeBinary || m_eTypeFile == eTypeText )
		fscanf_s(m_pFile, "%s", _sValue, _iSize);
	else
		return false;

	return true;
}