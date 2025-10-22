#pragma once

class AimbotMenu : public IMenuElement 
{
private:

	enum EItems
	{
		eAimbot,
		eAlwaysRCS,
		eRage,
		eSilent,
		eAim,
		eTSmooth,
		eFov,
		eSmooth,
		eRcs,
		eHitbox
	};

public:

	std::string Header()
	{
		return "AIMBOT";
	}

	std::vector<std::string> Items()
	{
		return std::vector<std::string>
		{ 
			"Aimbot",
			"Always On RCS",
			"Rage", 
			"pSilent", 
			"Aim Mode",
			"Toggle Smooth",
			"FOV", 
			"Smooth",
			"RCS",
			"Hitbox"
		};
	}

	char* Value( int index )
	{
		static std::string str;

		switch( index )
		{
		case eAimbot:
			return g_pAimbot->m_aimbot		? "ON" : "OFF";
		case eAlwaysRCS:
			return g_pAimbot->m_alwaysonrcs ? "ON" : "OFF";
		case eRage:
			return g_pAimbot->m_rage		? "ON" : "OFF";
		case eSilent:
			return g_pAimbot->m_psilent		? "ON" : "OFF";
		case eAim:
			return g_pAimbot->m_mode		? "DIS" : "FOV";
		case eTSmooth:
			return g_pAimbot->m_smooth		? "ON" : "OFF";
		case eFov:
			str = Utils::to_string_with_precision<float>( g_pAimbot->m_Fov );
			return( char* )str.c_str();
		case eSmooth:
			str = Utils::to_string_with_precision<float>( g_pAimbot->m_SmoothAmount );
			return( char* )str.c_str();
		case eRcs:
			str = Utils::to_string_with_precision<float>( g_pAimbot->m_PunchAmount );
			return( char* )str.c_str();
		case eHitbox:
			str = Utils::to_string_with_precision<int>( g_pAimbot->m_Hitbox, 0 );
			return( char* )str.c_str();
		default:
			return "N/A";
		}
	}

	void OnRightKey( int index )
	{
		switch( index )
		{
		case eAimbot:
			g_pAimbot->SetAimbot();
			break;
		case eAlwaysRCS:
			g_pAimbot->SetAlwaysOnRCS();
			break;
		case eRage:
			g_pAimbot->SetRage();
			break;
		case eSilent:
			g_pAimbot->SetPSilent();
			break;
		case eAim:
			g_pAimbot->SetMode();
			break;
		case eTSmooth:
			g_pAimbot->SetToggleSmooth();
			break;
		case eFov:
			g_pAimbot->m_Fov += 0.1;
			if( g_pAimbot->m_Fov > 360.0 )
				g_pAimbot->m_Fov = 360.0;
			break;
		case eSmooth:
			g_pAimbot->m_SmoothAmount += 0.05;
			break;
		case eRcs:
			g_pAimbot->m_PunchAmount += 0.05;
			if( g_pAimbot->m_PunchAmount > 2.0 )
				g_pAimbot->m_PunchAmount = 2.0;
			break;
		case eHitbox:
			g_pAimbot->m_Hitbox += 1;
			if( g_pAimbot->m_Hitbox > 30 )
				g_pAimbot->m_Hitbox = 30;
			break;
		default:
			break;
		}
	}

	void OnLeftKey( int index )
	{
		switch( index )
		{
		case eAimbot:
			g_pAimbot->SetAimbot();
			break;
		case eAlwaysRCS:
			g_pAimbot->SetAlwaysOnRCS();
			break;
		case eRage:
			g_pAimbot->SetRage();
			break;
		case eSilent:
			g_pAimbot->SetPSilent();
			break;
		case eAim:
			g_pAimbot->SetMode();
			break;
		case eTSmooth:
			g_pAimbot->SetToggleSmooth();
			break;
		case eFov:
			g_pAimbot->m_Fov -= 0.1;
			if( g_pAimbot->m_Fov < 0.0 )
				g_pAimbot->m_Fov = 0.0;
			break;
		case eSmooth:
			g_pAimbot->m_SmoothAmount -= 0.05;
			if( g_pAimbot->m_SmoothAmount < 0.0 )
				g_pAimbot->m_SmoothAmount = 0.0;
			break;
		case eRcs:
			g_pAimbot->m_PunchAmount -= 0.05;
			if( g_pAimbot->m_PunchAmount < 0.0 )
				g_pAimbot->m_PunchAmount = 0.0;
			break;
		case eHitbox:
			g_pAimbot->m_Hitbox -= 1;
			if( g_pAimbot->m_Hitbox < 0 )
				g_pAimbot->m_Hitbox = 0;
			break;
		default:
			break;
		}
	}
};