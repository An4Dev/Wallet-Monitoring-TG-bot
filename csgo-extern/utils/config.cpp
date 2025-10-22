#include "..\include.h"

Config* g_pConfig;

void Config::OnSetup()
{
	Load();
}

void Config::Save()
{
	g_pFiles->WriteBool(	"Aimbot",	"Aimbot",		g_pAimbot->m_aimbot );
	g_pFiles->WriteBool(	"Aimbot",	"AlwaysRCS",	g_pAimbot->m_alwaysonrcs );
	g_pFiles->WriteBool(	"Aimbot",	"Rage Mode",	g_pAimbot->m_rage );
	g_pFiles->WriteBool(	"Aimbot",	"pSilent",		g_pAimbot->m_psilent );
	g_pFiles->WriteBool(	"Aimbot",	"AimMode",		g_pAimbot->m_mode );
	g_pFiles->WriteFloat(	"Aimbot",	"FOV",			g_pAimbot->m_Fov );
	g_pFiles->WriteFloat(	"Aimbot",	"Smooth",		g_pAimbot->m_SmoothAmount );
	g_pFiles->WriteFloat(	"Aimbot",	"RCS",			g_pAimbot->m_PunchAmount );
	g_pFiles->WriteInt(		"Aimbot",	"Hitbox",		g_pAimbot->m_Hitbox );

	g_pFiles->WriteBool(	"Trigger",	"Trigger",		g_pTrigger->m_trigger );
	g_pFiles->WriteInt(		"Trigger",	"Hitbox",		g_pTrigger->GetHitbox() );

	g_pFiles->WriteBool(	"Visuals",	"Enemy ESP",	g_pESP->m_enemy );
	g_pFiles->WriteBool(	"Visuals",	"Friendly ESP",	g_pESP->m_friendly );
	g_pFiles->WriteBool(	"Visuals",	"Legit ESP",	g_pESP->m_legit );
	g_pFiles->WriteBool(	"Visuals",	"Sprites",		g_pESP->m_anime );
	g_pFiles->WriteBool(	"Visuals",	"ESP Box",		g_pESP->m_box );
	g_pFiles->WriteBool(	"Visuals",	"Names",		g_pESP->m_playername );
    g_pFiles->WriteBool(	"Visuals",	"Ranks",		g_pESP->m_ranks );
	g_pFiles->WriteBool(	"Visuals",	"Health Bar",	g_pESP->m_healthbar );
	g_pFiles->WriteBool(	"Visuals",	"Weapon",		g_pESP->m_weapon );
	g_pFiles->WriteBool(	"Visuals",	"Distance",		g_pESP->m_distance );
	g_pFiles->WriteBool(	"Visuals",	"Aimspot",		g_pESP->m_aimspot );
	g_pFiles->WriteBool(	"Visuals",	"Skeleton",		g_pESP->m_skeleton );
	g_pFiles->WriteBool(	"Visuals",	"Hitboxes",		g_pESP->m_hitbox );

	g_pFiles->WriteBool(	"Misc",		"BHop",			g_pMisc->m_bhop );
	g_pFiles->WriteBool(	"Misc",		"AutoStrafe",	g_pMisc->m_autostrafe );
	g_pFiles->WriteBool(	"Misc",		"No Flash",		g_pMisc->m_noflash );
	g_pFiles->WriteBool(	"Misc",		"Fake Lag",		g_pMisc->m_fakelag );
	g_pFiles->WriteBool(	"Misc",		"No Hands",		g_pMisc->m_nohands );
	g_pFiles->WriteBool(	"Misc",		"Skin Changer",	g_pMisc->m_skinchanger );
	g_pFiles->WriteInt(		"Misc",		"Skin ID",		g_pMisc->m_skinId );
}

void Config::Load()
{
	g_pAimbot->m_aimbot			= g_pFiles->ReadBool(	"Aimbot", "Aimbot" );
	g_pAimbot->m_alwaysonrcs	= g_pFiles->ReadBool(	"Aimbot", "AlwaysRCS" );
	g_pAimbot->m_rage			= g_pFiles->ReadBool(	"Aimbot", "Rage Mode" );
	g_pAimbot->m_psilent		= g_pFiles->ReadBool(	"Aimbot", "pSilent" );
	g_pAimbot->m_mode			= g_pFiles->ReadBool(	"Aimbot", "AimMode" );
	g_pAimbot->m_Fov			= g_pFiles->ReadFloat(	"Aimbot", "FOV" );
	g_pAimbot->m_SmoothAmount	= g_pFiles->ReadFloat(	"Aimbot", "Smooth" );
	g_pAimbot->m_PunchAmount	= g_pFiles->ReadFloat(	"Aimbot", "RCS" );
	g_pAimbot->m_Hitbox			= g_pFiles->ReadInt(	"Aimbot", "HitBox" );

	g_pTrigger->m_trigger		= g_pFiles->ReadBool(	"Trigger", "Trigger" );
	g_pTrigger->SetHitboxes( {	  g_pFiles->ReadInt(	"Trigger", "Hitbox" ) } );

	g_pESP->m_enemy				= g_pFiles->ReadBool(	"Visuals", "Enemy ESP" );
	g_pESP->m_friendly			= g_pFiles->ReadBool(	"Visuals", "Friendly ESP" );
	g_pESP->m_legit				= g_pFiles->ReadBool(	"Visuals", "Legit ESP" );
	g_pESP->m_anime				= g_pFiles->ReadBool(	"Visuals", "Sprites" );
	g_pESP->m_box				= g_pFiles->ReadBool(	"Visuals", "ESP Box" );
	g_pESP->m_playername		= g_pFiles->ReadBool(	"Visuals", "Names" );
	g_pESP->m_ranks				= g_pFiles->ReadBool(	"Visuals", "Ranks" );
	g_pESP->m_healthbar			= g_pFiles->ReadBool(	"Visuals", "Health Bar" );
	g_pESP->m_weapon			= g_pFiles->ReadBool(	"Visuals", "Weapon" );
	g_pESP->m_distance			= g_pFiles->ReadBool(	"Visuals", "Distance" );
	g_pESP->m_aimspot			= g_pFiles->ReadBool(	"Visuals", "Aimspot" );
	g_pESP->m_skeleton			= g_pFiles->ReadBool(	"Visuals", "Skeleton" );
	g_pESP->m_hitbox			= g_pFiles->ReadBool(	"Visuals", "Hitboxes" );

	g_pMisc->m_bhop				= g_pFiles->ReadBool(	"Misc",		"BHop" );
	g_pMisc->m_autostrafe		= g_pFiles->ReadBool(	"Misc",		"AutoStrafe" );
	g_pMisc->m_noflash			= g_pFiles->ReadBool(	"Misc",		"No Flash" );
	g_pMisc->m_fakelag			= g_pFiles->ReadBool(	"Misc",		"Fake Lag" );
	g_pMisc->m_nohands			= g_pFiles->ReadBool(	"Misc",		"No Hands" );
	g_pMisc->m_skinchanger		= g_pFiles->ReadBool(	"Misc",		"Skin Changer" );
	g_pMisc->m_skinId			= g_pFiles->ReadInt(	"Misc",		"Skin ID" );
}