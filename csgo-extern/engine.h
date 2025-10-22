#pragma once

enum SIGNONSTATE
{
	SIGNONSTATE_NONE = 0,
	SIGNONSTATE_CHALLENGE = 1,
	SIGNONSTATE_CONNECTED = 2,
	SIGNONSTATE_NEW = 3,
	SIGNONSTATE_PRESPAWN = 4,
	SIGNONSTATE_SPAWN = 5,
	SIGNONSTATE_FULL = 6,
	SIGNONSTATE_CHANGELEVEL = 7
};

class Engine
{
public:
	Engine();
	~Engine();

	int			GetLocalPlayer();

	void		GetViewAngles( Vector3& viewangles );
	void		SetViewAngles( const Vector3& viewangles );
	Vector3		RetViewAngles();
	bool		IsConnected();
	bool		IsInGame();

	void        SetSendPacket( bool mode );
	bool		GetSendPacket();

	void        SetViewAnglesSilent( const Vector3& viewangles );
	void        SetViewAnglesPSilent( Vector3& viewangles );

	bool		WorldToScreen( const Vector3& vIn, Vector3& vOut );

	const char* GetMapName();

	void		ForceUpdate();

	void		ClientCMD( const char* command );

public:
	char		m_pMapName[ 32 ];
private:
	int m_chokedPackets = 0;
	bool m_sendpackets;
};

extern Engine* g_pEngine;