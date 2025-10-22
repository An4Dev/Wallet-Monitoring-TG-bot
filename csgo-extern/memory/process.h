#pragma once

#define SAFE_DELETE_VECTOR(vec)	if (!vec.empty()) { \
									for (auto& slot : vec) { \
										delete slot; \
										slot = nullptr; \
									} \
								}

class Process
{
public:
	Process();
	~Process();

	bool					Attach( const std::string& ExeName );
	void					Detach();

	bool					Read( DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize );
	bool					Write( DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize );

	Module*					GetModule( const std::string& ImageName );
	HMODULE					LoadRemote( const std::string& ImageName );

	void                    SetWindow( HWND window );
	HWND                    GetWindow();

	DWORD_PTR				Scan( DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask );

	template<typename T>
	T Read( DWORD_PTR dwAddress, const T& tDefault = T() )
	{
		T tRet;

		if( !Read( dwAddress, &tRet, sizeof( T ) ) )
		{
			return tDefault;
		}

		return tRet;
	}

	template<typename T>
	bool Write( DWORD_PTR dwAddress, const T& tValue )
	{
		return Write( dwAddress, &tValue, sizeof( T ) );
	}

	template<typename T>
	bool WriteProtected( DWORD_PTR dwAddress, const T& tValue )
	{
		DWORD_PTR oldProtect;
		VirtualProtectEx( m_hProcess, ( LPVOID )dwAddress, sizeof( T ), PAGE_EXECUTE_READWRITE, &oldProtect );
		Write( dwAddress, &tValue, sizeof( T ) );
		VirtualProtectEx( m_hProcess, ( LPVOID )dwAddress, sizeof( T ), oldProtect, NULL );
		return true;
	}

private:
	bool					DumpModList();
	DWORD					GetProcessIdByName( const std::string& name );

private:
	DWORD					m_dwProcessId = NULL;
	std::vector<Module*>	m_pModList;
	HWND					m_hWindow = NULL;
public:
	HANDLE					m_hProcess = NULL;
};

extern Process*	process;
extern Module*	client;
extern Module*	engine;
extern Module* csgobase;

