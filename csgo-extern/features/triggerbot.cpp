#include "..\include.h"

Trigger* g_pTrigger = new Trigger();

void Trigger::HitBotTriggerBot( int iIndex )
{
	if( GetAsyncKeyState( 0x02 ) && m_trigger )
	{
		Vector3 viewDirection = g_pMath->AngleToDirection( g_pEngine->RetViewAngles() );
		Ray viewRay( LocalEntity.GetEyePosition(), viewDirection );

		float distance;

		for( auto hitbox : m_hitboxes )
		{
			if( viewRay.IntersectWithOBB( EntityList[ iIndex ].GetBoneMatrix( Hitbox[ hitbox ].iBone ), Hitbox[ hitbox ].vMin, Hitbox[ hitbox ].vMax, distance ) )
				Shoot();
		}
	}
}

void Trigger::SetTriggerbot()
{
	m_trigger = !m_trigger;
}

bool Trigger::GetTriggerbot()
{
	return m_trigger;
}

void Trigger::SetHitboxes( std::vector<int> hitboxes )
{
	m_hitboxes = hitboxes;
}

int Trigger::GetHitbox()
{
	return m_hitboxes[ 0 ];
}

void Trigger::ShitTriggerbot()
{
	if( GetAsyncKeyState( 0x02 ) )
	{
		int index = LocalEntity.GetCrosshairEnt();

		if( index >= 0 && index < g_pGlobals->GetMaxClients() )
		{
			if( EntityList[ index ].IsValid() &&
				!EntityList[ index ].IsDead() &&
				EntityList[ index ].GetTeamNum() != LocalEntity.GetTeamNum() )
			{
				Shoot();
			}
		}
	}
}

void Trigger::Shoot()
{
	process->WriteProtected<BYTE>( g_pStatic->ConvarAttack, 5 );
	Sleep( 1 );
	process->WriteProtected<BYTE>( g_pStatic->ConvarAttack, 4 );
}

void Trigger::Update()
{
	m_hitboxes.clear();

	if( m_head )
		m_hitboxes.push_back( HITBOX_HEAD );

	if( m_chest )
	{
		m_hitboxes.push_back( HITBOX_NECK );
		m_hitboxes.push_back( HITBOX_L_CLAVICLE );
		m_hitboxes.push_back( HITBOX_R_CLAVICLE );
		m_hitboxes.push_back( HITBOX_SPINE3 );
		m_hitboxes.push_back( HITBOX_SPINE4 );
	}

	if( m_body )
	{
		m_hitboxes.push_back( HITBOX_SPINE1 );
		m_hitboxes.push_back( HITBOX_SPINE2 );
		m_hitboxes.push_back( HITBOX_PELVIS );
	}

	if( m_arms )
	{
		m_hitboxes.push_back( HITBOX_L_HAND );
		m_hitboxes.push_back( HITBOX_L_UPPERARM );
		m_hitboxes.push_back( HITBOX_L_FOREARM );
		m_hitboxes.push_back( HITBOX_R_HAND );
		m_hitboxes.push_back( HITBOX_R_UPPERARM );
		m_hitboxes.push_back( HITBOX_R_FOREARM );
	}

	if( m_legs )
	{
		m_hitboxes.push_back( HITBOX_L_FOOT );
		m_hitboxes.push_back( HITBOX_L_CALF );
		m_hitboxes.push_back( HITBOX_L_THIGH );
		m_hitboxes.push_back( HITBOX_R_FOOT );
		m_hitboxes.push_back( HITBOX_R_CALF );
		m_hitboxes.push_back( HITBOX_R_THIGH );
	}
}

void Trigger::ToggleHead()
{
	m_head = !m_head;
	Update();
}

void Trigger::ToggleChest()
{
	m_chest = !m_chest;
	Update();
}

void Trigger::ToggleBody()
{
	m_body = !m_body;
	Update();
}

void Trigger::ToggleArms()
{
	m_arms = !m_arms;
	Update();
}

void Trigger::ToggleLegs()
{
	m_legs = !m_legs;
	Update();
}
