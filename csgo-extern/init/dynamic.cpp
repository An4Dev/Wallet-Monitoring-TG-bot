#include "..\include.h"

Dynamic* g_pDynamic = new Dynamic();

void Dynamic::OnSetup()
{
	m_lifeState				= GetNetVar( "DT_BasePlayer", "m_lifeState" );
	m_iHealth				= GetNetVar( "DT_BasePlayer", "m_iHealth" );
	m_iTeamNum				= GetNetVar( "DT_BaseEntity", "m_iTeamNum" );
	m_vecOrigin				= GetNetVar( "DT_CSLocalPlayerExclusive", "m_vecOrigin" );
	m_vecViewOffset			= GetNetVar( "DT_LocalPlayerExclusive", "m_vecViewOffset[0]" );
	m_vecVelocity			= 0x110;
	m_fFlags				= GetNetVar( "DT_BasePlayer", "m_fFlags" );
	m_nTickBase				= GetNetVar( "DT_BasePlayer", "m_nTickBase" );
	m_Local					= GetNetVar( "DT_LocalPlayerExclusive", "m_Local" );
	m_aimPunchAngle			= 0x13e8;
	m_iCrossHairID			= GetNetVar( "DT_Local", "m_iCrossHairID" );
	m_hActiveWeapon			= GetNetVar( "DT_BaseCombatCharacter", "m_hActiveWeapon" );
	m_angEyeAngles			= GetNetVar( "DT_CSPlayer", "m_angEyeAngles[0]" );
	m_ArmorValue			= GetNetVar( "DT_CSPlayer", "m_ArmorValue" );
	m_flFlashDuration		= GetNetVar( "DT_CSPlayer", "m_flFlashDuration" ); //Use the netvars if it's a fucking netvar
	m_iShotsFired			= GetNetVar( "DT_CSLocalPlayerExclusive", "m_iShotsFired" );
	m_nForceBone			= GetNetVar( "DT_BaseAnimating", "m_nForceBone" );
	m_iCompetitiveRanking	= GetNetVar( "DT_CSPlayerResource", "m_iCompetitiveRanking" );
	m_iCompetitiveWins		= GetNetVar( "DT_CSPlayerResource", "m_iCompetitiveWins" );
	m_BoneMatrix			= m_nForceBone + 0x1C;
	m_pModel				= 0x6C;
	m_szArmsModel			= GetNetVar( "DT_BasePlayer", "m_szArmsModel" );

	m_iClip1				= GetNetVar( "DT_LocalWeaponData", "m_iClip1" );
	m_fAccuracyPenalty		= GetNetVar( "DT_WeaponCSBase", "m_fAccuracyPenalty" );
	m_iWeaponId				= m_fAccuracyPenalty + 0x20; // moved up by 1
	m_flNextPrimaryAttack	= 0x159c;
}

DWORD_PTR Dynamic::GetNetVar( const std::string& Data, const std::string& Name )
{
	DWORD_PTR dwRet = NULL;

	for( auto pList = g_pInterface->m_pClient->GetList(); pList; pList = pList->m_pNext )
	{
		RecvTable* Table = pList->m_pRecvTable;

		if( !Table )
			continue;

		if( !Table->GetNumProps() )
			continue;

		dwRet = GetEntry( Data, Name, Table );

		if( dwRet )
			break;
	}
	return dwRet;
}

DWORD_PTR Dynamic::GetEntry( const std::string& Data, const std::string& Name, RecvTable* Table )
{
	DWORD_PTR dwRet = NULL;
	RecvProp* Prop = NULL;
	RecvTable* Child = NULL;

	for( int i = 0; i < Table->GetNumProps(); i++ )
	{
		Prop = Table->GetPropA( i );

		if( !Prop )
			continue;

		Child = Prop->GetDataTable();

		if( Child && Child->GetNumProps() )
			dwRet += GetEntry( Data, Name, Child );

		if( !Data.compare( Table->GetName() ) && !Name.compare( Prop->GetName() ) )
		{
			dwRet += Prop->GetOffset();
			break;
		}
	}
	return dwRet;
}