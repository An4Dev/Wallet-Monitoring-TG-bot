#pragma once

class VisualsMenu : public IMenuElement
{
private:

	enum EItems
	{
		eEnemy,
		eFriendly,
		eLegit,
		eSprite,
		eBox,
		eName,
		eRank,
		eHealth,
		eWeapon,
		eDistance,
		eAimspot,
		eSkeleton,
		eHitboxes
	};

public:

	std::string Header()
	{
		return "VISUALS";
	}

	std::vector<std::string> Items()
	{
		return std::vector<std::string>
		{ 
			"Enemy ESP", 
			"Friendly ESP", 
			"Legit ESP", 
			"Sprites", 
			"ESP Box", 
			"Names", 
			"Ranks", 
			"Health Bar",
			"Weapon",
			"Distance",
			"Aimspot", 
			"Skeleton",
			"Hitboxes" 
		};
	}

	char* Value( int index )
	{
		static std::string str;

		switch( index )
		{
		case eEnemy:
			return g_pESP->GetEnemy()		? "ON" : "OFF";
		case eFriendly:
			return g_pESP->GetFriendly()	? "ON" : "OFF";
		case eLegit:
			return g_pESP->GetLegit()		? "ON" : "OFF";
		case eSprite:
			return g_pESP->GetAnime()		? "ON" : "OFF";
		case eBox:
			return g_pESP->GetBox()			? "ON" : "OFF";
		case eName:
			return g_pESP->GetPlayerName()	? "ON" : "OFF";
		case eRank:
			return g_pESP->GetRank()		? "ON" : "OFF";
		case eHealth:
			return g_pESP->GetHealthBar()	? "ON" : "OFF";
		case eWeapon:
			return g_pESP->GetWeapon()		? "ON" : "OFF";
		case eDistance:
			return g_pESP->GetDistance()	? "ON" : "OFF";
		case eAimspot:
			return g_pESP->GetAimspot()		? "ON" : "OFF";
		case eSkeleton:
			return g_pESP->GetSkeleton()	? "ON" : "OFF";
		case eHitboxes:
			return g_pESP->GetHitbox()		? "ON" : "OFF";
		default:
			return "N/A";
		}
	}

	void OnRightKey( int index )
	{
		switch( index )
		{
		case eEnemy:
			g_pESP->SetEnemy();
			break;
		case eFriendly:
			g_pESP->SetFriendly();
			break;
		case eLegit:
			g_pESP->SetLegit();
			break;
		case eSprite:
			g_pESP->SetAnime();
			break;
		case eBox:
			g_pESP->SetBox();
			break;
		case eName:
			g_pESP->SetPlayerName();
			break;
		case eRank:
			g_pESP->SetRank();
			break;
		case eHealth:
			g_pESP->SetHealthBar();
			break;
		case eWeapon:
			g_pESP->SetWeapon();
			break;
		case eDistance:
			g_pESP->SetDistance();
			break;
		case eAimspot:
			g_pESP->SetAimspot();
			break;
		case eSkeleton:
			g_pESP->SetSkeleton();
			break;
		case eHitboxes:
			g_pESP->SetHitbox();
			break;
		default:
			break;
		}
	}

	void OnLeftKey( int index )
	{
		switch( index )
		{
		case eEnemy:
			g_pESP->SetEnemy();
			break;
		case eFriendly:
			g_pESP->SetFriendly();
			break;
		case eLegit:
			g_pESP->SetLegit();
			break;
		case eSprite:
			g_pESP->SetAnime();
			break;
		case eBox:
			g_pESP->SetBox();
			break;
		case eName:
			g_pESP->SetPlayerName();
			break;
		case eRank:
			g_pESP->SetRank();
			break;
		case eHealth:
			g_pESP->SetHealthBar();
			break;
		case eWeapon:
			g_pESP->SetWeapon();
			break;
		case eDistance:
			g_pESP->SetDistance();
			break;
		case eAimspot:
			g_pESP->SetAimspot();
			break;
		case eSkeleton:
			g_pESP->SetSkeleton();
			break;
		case eHitboxes:
			g_pESP->SetHitbox();
			break;
		default:
			break;
		}
	}
};