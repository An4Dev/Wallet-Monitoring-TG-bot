#pragma once

class MiscMenu : public IMenuElement
{
private:

	enum EItems
	{
		eBHop,
		eStrafe,
		eNoFlash,
		eFakeLag,
		eNoHands,
		eSkinChanger,
		eSkinID,
		eDerank,
		eInsult, 
		eIrc,
		eYiff
	};

public:

	std::string Header()
	{
		return "MISC";
	}

	std::vector<std::string> Items()
	{
		return std::vector<std::string>
		{
			"Bunny Hop",
			"Auto Strafe",
			"No Flash",
			"Fake Lag", 
			"No Hands",
			"Skin Changer",
			"Set Skin ID",
			"Derank",
			"Insult Generator",
			"Irc Chat Reciever",
			"Yiff"
		};
	}

	char* Value( int index )
	{
		static std::string str;

		switch( index )
		{
		case eBHop:
			return g_pMisc->GetBHop()			? "ON" : "OFF";
		case eStrafe:
			switch( g_pMisc->GetAutoStrafe() )
			{
			case 0:
				return "OFF";
			case 1:
				return "^";
			case 2:
				return "<";
			case 3:
				return "v";
			case 4:
				return ">";
			default:
				return "";
			}
		case eNoFlash:
			return g_pMisc->GetNoFlash()		? "ON" : "OFF";
		case eFakeLag:
			return g_pMisc->GetFakeLag()		? "ON" : "OFF";
		case eNoHands:
			return g_pMisc->GetNoHands()		? "ON" : "OFF";
		case eSkinChanger:
			return g_pMisc->GetSkinChanger()	? "ON" : "OFF";
		case eSkinID:
			str = Utils::to_string_with_precision<int>( g_pMisc->GetSkinID(), 0 );
			return( char* )str.c_str();
		case eDerank:
			return g_pMisc->m_derank			? "ON" : "OFF";
		case eInsult:
			return g_pMisc->m_insultgen			? "ON" : "OFF";
		case eIrc:
			return g_pMisc->m_ircchatter		? "ON" : "OFF";
		case eYiff:
			return g_pMisc->m_yiff				? "ON" : "OFF";
		default:
			return "N/A";
		}
	}

	void OnRightKey( int index )
	{
		switch( index )
		{
		case eBHop:
			g_pMisc->SetBHop();
			break;
		case eStrafe:
		{
			int ASDir = g_pMisc->GetAutoStrafe();
			g_pMisc->SetAutoStrafe(ASDir += 1);
			if (ASDir > 4)
			{
				ASDir = 4;
				g_pMisc->SetAutoStrafe(ASDir);
			}
			break;
		}
			break;
		case eNoFlash:
			g_pMisc->SetNoFlash();
			break;
		case eFakeLag:
			g_pMisc->SetFakeLag();
			break;
		case eNoHands:
			g_pMisc->SetNoHands();
			break;
		case eSkinChanger:
			g_pMisc->SetSkinChanger();
			break;
		case eSkinID:
		{
			int skinID = g_pMisc->GetSkinID();
			g_pMisc->SetSkinID( skinID += 1 );
			if( skinID > 500 )
			{
				skinID = 500;
				g_pMisc->SetSkinID( skinID );
			}
			break;
		}
		case eDerank:
			g_pMisc->SetDerank();
			break;
		case eInsult:
			g_pMisc->SetInsultGen();
			break;
		case eIrc:
			g_pMisc->SetIrcChatter();
			break;
		case eYiff:
			g_pMisc->SetYiff();
			break;
		default:
			break;
		}
	}

	void OnLeftKey( int index )
	{
		switch( index )
		{
		case eBHop:
			g_pMisc->SetBHop();
			break;
		case eStrafe:
		{
			int ASDir = g_pMisc->GetAutoStrafe();
			g_pMisc->SetAutoStrafe(ASDir -= 1);
			if (ASDir < 0)
			{
				ASDir = 0;
				g_pMisc->SetAutoStrafe(ASDir);
			}
			break;
		}
		case eNoFlash:
			g_pMisc->SetNoFlash();
			break;
		case eFakeLag:
			g_pMisc->SetFakeLag();
			break;
		case eNoHands:
			g_pMisc->SetNoHands();
			break;
		case eSkinChanger:
			g_pMisc->SetSkinChanger();
			break;
		case eSkinID:
		{
			int skinID = g_pMisc->GetSkinID();
			g_pMisc->SetSkinID( skinID -= 1 );
			if( skinID < 0 )
			{
				skinID = 0;
				g_pMisc->SetSkinID( skinID );
			}
			break;
		}
		case eDerank:
			g_pMisc->SetDerank();
			break;
		case eInsult:
			g_pMisc->SetInsultGen();
			break;
		case eIrc:
			g_pMisc->SetIrcChatter();
			break;
		case eYiff:
			g_pMisc->SetYiff();
			break;
		default:
			break;
		}
	}
};