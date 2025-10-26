#include "include.h"

/*will still skull fuck you tbh*/

Globals* g_pGlobals = new Globals;

float Globals::GetCurTime()
{
	return process->Read<float>( g_pStatic->GlobalPointer + 0x10 );
}

float Globals::GetFrameTime()
{
	return process->Read<float>( g_pStatic->GlobalPointer + 0x14 );
}

int	Globals::GetMaxClients()
{
	return process->Read<int>( g_pStatic->GlobalPointer + 0x18 );
}

float Globals::GetIntervalPerTick()
{
	return process->Read<float>( g_pStatic->GlobalPointer + 0x20 );
}

float Globals::GetInterpolationAmount()
{
	return process->Read<float>( g_pStatic->GlobalPointer + 0x24 );
}