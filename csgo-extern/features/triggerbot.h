#pragma once

class Trigger
{
public:
	Trigger()
	{
		m_hitboxes = { HITBOX_HEAD };
	}

	void HitBotTriggerBot( int iIndex );
	void ShitTriggerbot();

	void SetTriggerbot();
	bool GetTriggerbot();

	void SetHitboxes( std::vector<int> hitboxes );
	void SetHitbox( int bone );
	int  GetHitbox();

	void ToggleHead();
	void ToggleChest();
	void ToggleBody();
	void ToggleArms();
	void ToggleLegs();

private:
	void Shoot();
	void Update();

public:
	bool m_trigger = true;

	bool m_head = true;
	bool m_chest = false; 
	bool m_body = false;
	bool m_arms = false;
	bool m_legs = false;

private:
	std::vector<int> m_hitboxes;

};
extern Trigger* g_pTrigger;