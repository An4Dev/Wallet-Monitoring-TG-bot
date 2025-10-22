#pragma once // opps

class Globals
{
public:
	float	GetCurTime();
	float	GetFrameTime();
	int		GetMaxClients();
	float	GetIntervalPerTick();
	float   GetInterpolationAmount();

private:
	/*
	struct CGlobalVarsBase_t
	{
		0x00	float    realtime;
		0x04	int      framecount;
		0x08	float    absoluteframetime;
		0x0C	float    absoluteframestarttimestddev;
		0x10	float    curtime;
		0x14	float    frametime;
		0x18	int      maxClients;
		0x1C	int      tickcount;
		0x20	float    interval_per_tick;
		0x24	float    interpolation_amount;
		0x28	int      simTicksThisFrame;
		0x2C	int      network_protocol;
		0x30	void*    pSaveData;
		0x34	bool     m_bClient;
		0x38	int      nTimestampNetworkingBase;
		0x3C	int      nTimestampRandomizeWindow;
	} m_pGlobals;
	*/

	
};

extern Globals* g_pGlobals;