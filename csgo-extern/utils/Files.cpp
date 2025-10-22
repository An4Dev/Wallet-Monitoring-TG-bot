#include "..\include.h"

Files* g_pFiles = new Files();

void Files::OnSetup( const char* fileName, const char* fileDir )
{
	memset( m_fileName, 0x00, 255 );
	strcpy( m_fileName, fileDir );
	strcat( m_fileName, fileName );
}

int Files::ReadInt( char* element, char* item )
{
	return GetPrivateProfileIntA( element, item, NULL, m_fileName );
}

float Files::ReadFloat( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return atof( result );
}

bool Files::ReadBool( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return ( strcmp( result, "True" ) == 0 || strcmp( result, "true" ) == 0 ) ? true : false;
}

char* Files::ReadChar( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return result;
}

void Files::WriteInt( char* element, char* item, int value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%d", value );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteFloat( char* element, char* item, float value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%f", value );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteBool( char* element, char* item, bool value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%s", value ? "true" : "false" );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteChar( char* element, char* item, char* value )
{
	WritePrivateProfileStringA( element, item, value, m_fileName );
}