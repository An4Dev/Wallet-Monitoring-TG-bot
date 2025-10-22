#pragma once

class TriggerMenu : public IMenuElement
{
private:

	enum EItems
	{
		eTrigger,
		eHead,
		eChest,
		eBody,
		eArms,
		eLegs
	};

public:

	std::string Header()
	{
		return "TRIGGERBOT";
	}

	std::vector<std::string> Items()
	{
		return std::vector<std::string>
		{
			"Triggerbot",
			"Head",
			"Chest",
			"Body",
			"Arms",
			"Legs"
		};
	}

	char* Value( int index )
	{
		static std::string str;

		switch( index )
		{
		case eTrigger:
			return g_pTrigger->m_trigger	? "ON" : "OFF";
		case eHead:
			return g_pTrigger->m_head		? "ON" : "OFF";
		case eChest:
			return g_pTrigger->m_chest		? "ON" : "OFF";
		case eBody:
			return g_pTrigger->m_body		? "ON" : "OFF";
		case eArms:
			return g_pTrigger->m_arms		? "ON" : "OFF";
		case eLegs:
			return g_pTrigger->m_legs		? "ON" : "OFF";
		default:
			return "N/A";
		}
	}

	void OnRightKey( int index )
	{
		switch( index )
		{
		case eTrigger:
			g_pTrigger->SetTriggerbot();
			break;
		case eHead:
			g_pTrigger->ToggleHead();
			break;
		case eChest:
			g_pTrigger->ToggleChest();
			break;
		case eBody:
			g_pTrigger->ToggleBody();
			break;
		case eArms:
			g_pTrigger->ToggleArms();
			break;
		case eLegs:
			g_pTrigger->ToggleLegs();
			break;
		default:
			break;
		}
	}

	void OnLeftKey( int index )
	{
		switch( index )
		{
		case eTrigger:
			g_pTrigger->SetTriggerbot();
			break;
		case eHead:
			g_pTrigger->ToggleHead();
			break;
		case eChest:
			g_pTrigger->ToggleChest();
			break;
		case eBody:
			g_pTrigger->ToggleBody();
			break;
		case eArms:
			g_pTrigger->ToggleArms();
			break;
		case eLegs:
			g_pTrigger->ToggleLegs();
			break;
		default:
			break;
		}
	}

};