#include "include.h"

Engine* g_pEngine = new Engine();

Engine::Engine()
{}

Engine::~Engine()
{}

bool Engine::IsConnected()
{
	if( !g_pStatic->EnginePointer )
		return false;
	return ( process->Read<int>( g_pStatic->EnginePointer + 0xE8 ) == ( int )SIGNONSTATE_CONNECTED );
}

bool Engine::IsInGame()
{
	if( !g_pStatic->EnginePointer )
		return false;
	return ( process->Read<int>( g_pStatic->EnginePointer + 0xE8 ) == ( int )SIGNONSTATE_FULL );
}

int Engine::GetLocalPlayer()
{
	if( !g_pStatic->EnginePointer )
		return -1;
	return process->Read<int>( g_pStatic->EnginePointer + 0x15C );
}

void Engine::GetViewAngles( Vector3& viewangles )
{
	if( !g_pStatic->EnginePointer )
		return;
	viewangles = process->Read<Vector3>( g_pStatic->EnginePointer + 0x4CE0 );
}

void Engine::SetViewAngles( const Vector3& viewangles )
{
	if( !g_pStatic->EnginePointer || viewangles.IsZero() )
		return;
	process->Write<Vector3>( g_pStatic->EnginePointer + 0x4CE0, viewangles );
}

Vector3 Engine::RetViewAngles()
{
	if( !g_pStatic->EnginePointer )
		return { 0, 0, 0 };
	return process->Read<Vector3>( g_pStatic->EnginePointer + 0x4CE0 );
}

void Engine::SetSendPacket( bool mode )
{
	if( mode )
	{
		process->WriteProtected<BYTE>( g_pStatic->SendPacket, 1 );
	}
	else
	{
		process->WriteProtected<BYTE>( g_pStatic->SendPacket, 0 );
	}
}


// Played around with this,
// -Works 100% for low fov, so can be used for a legit bot
// -Works 100% client side, good for raging
// -Full FOV does not fully work demo/spec side, twitches at times because 
//		we're not inside the same thread with the same timings
// 
//  I feel like I've broken this :/
void Engine::SetViewAnglesPSilent( Vector3& viewangles )
{
	if( !g_pAimbot->IsAbleToShoot() )
		return;

	int userCMDSequenceNum = 0;
	Vector3 oldAngles;
	DWORD_PTR userCMD = g_pStatic->userCMD;

	int curSequenceNum = process->Read<int>( g_pStatic->EnginePointer + 0x4C7C ) + 1;
	userCMD += ( curSequenceNum % 150 ) * 0x64;

	SetSendPacket( false );

	while( userCMDSequenceNum != curSequenceNum )
	{
		g_pEngine->GetViewAngles( oldAngles );
		userCMDSequenceNum = process->Read<int>( userCMD + 0x4 );
	}

	g_pMath->ClampAngle( viewangles ); // Just too make sure!!

	for( int i = 0; i < 20; i++ )  // Called outside of a hook so just keep setting it untill we hit the timing
	{
		process->Write<Vector3>( userCMD + 0xC, viewangles );
	}

	g_pEngine->SetViewAngles( oldAngles ); // Set view angles asap 
	Sleep( 6 );	// Changed from 7 to 5, could be the reason for shooting twice?

	SetSendPacket( true ); 
}

bool Engine::WorldToScreen( const Vector3& vIn, Vector3& vOut )
{
	Matrix4x4 vMatrix = process->Read<Matrix4x4>( client->GetImage() + g_pStatic->WorldToScreenMatrix );

	vOut[ 0 ] = vMatrix.m[ 0 ][ 0 ] * vIn[ 0 ] + vMatrix.m[ 0 ][ 1 ] * vIn[ 1 ] + vMatrix.m[ 0 ][ 2 ] * vIn[ 2 ] + vMatrix.m[ 0 ][ 3 ];
	vOut[ 1 ] = vMatrix.m[ 1 ][ 0 ] * vIn[ 0 ] + vMatrix.m[ 1 ][ 1 ] * vIn[ 1 ] + vMatrix.m[ 1 ][ 2 ] * vIn[ 2 ] + vMatrix.m[ 1 ][ 3 ];

	float w = vMatrix.m[ 3 ][ 0 ] * vIn[ 0 ] + vMatrix.m[ 3 ][ 1 ] * vIn[ 1 ] + vMatrix.m[ 3 ][ 2 ] * vIn[ 2 ] + vMatrix.m[ 3 ][ 3 ];

	if( w < 0.01 )
		return false;

	float invw = 1.0f / w;

	vOut[ 0 ] *= invw;
	vOut[ 1 ] *= invw;

	int width, height;

	g_pOverlay->GetScreenSize( &width, &height );

	float x = ( float )width / 2;
	float y = ( float )height / 2;

	x += 0.5 * vOut[ 0 ] * ( float )width + 0.5;
	y -= 0.5 * vOut[ 1 ] * ( float )height + 0.5;

	vOut[ 0 ] = x;
	vOut[ 1 ] = y;

	return true;
}

const char* Engine::GetMapName()
{
	if( !g_pStatic->EnginePointer )
		return nullptr;
	process->Read( g_pStatic->EnginePointer + 0x16D, m_pMapName, sizeof( char[ 64 ] ) );
	return m_pMapName;
}

void Engine::ForceUpdate()
{
	if( process->Read<int>( g_pStatic->EnginePointer + 0x154 ) != -1 )
	{
		process->Write<int>( g_pStatic->EnginePointer + 0x154, -1 );
	}
}

void Engine::ClientCMD( const char* command )
{
	LPVOID addr = ( LPVOID )g_pStatic->ClientCMD;
	LPVOID vCommand = ( LPVOID )VirtualAllocEx( process->m_hProcess, NULL, strlen( command ) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
	WriteProcessMemory( process->m_hProcess, vCommand, command, strlen( command ), NULL );
	HANDLE hThread = CreateRemoteThread( process->m_hProcess, NULL, NULL, ( LPTHREAD_START_ROUTINE )addr, vCommand, NULL, NULL );
	WaitForSingleObject( hThread, INFINITE );
	VirtualFreeEx( process->m_hProcess, vCommand, strlen( command ) + 1, MEM_RELEASE );
}
