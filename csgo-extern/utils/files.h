#pragma once

class Files
{
public:
	void	OnSetup( const char* fileName, const char* fileDir );

	int		ReadInt( char* element, char* item );
	float	ReadFloat( char* element, char* item );
	bool	ReadBool( char* element, char* item );
	char*	ReadChar( char* element, char* item );

	void	WriteInt( char* element, char* item, int value );
	void	WriteFloat( char* element, char* item, float value );
	void	WriteBool( char* element, char* item, bool value );
	void	WriteChar( char* element, char* item, char* value );

private:
	char m_fileName[ 255 ];
};

extern Files* g_pFiles;