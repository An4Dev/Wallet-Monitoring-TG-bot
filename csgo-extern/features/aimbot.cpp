#include "..\include.h"

Aimbot* g_pAimbot = new Aimbot();

bool Aimbot::IsAbleToShoot()
{
	float m_flNextPrimaryAttack = process->Read< float >( LocalEntity.GetActiveWeaponBase() + g_pDynamic->m_flNextPrimaryAttack );
	float flServerTime = LocalEntity.GetTickBase() * g_pGlobals->GetIntervalPerTick();

	return ( !( m_flNextPrimaryAttack > flServerTime ) );
}

void Aimbot::FindTarget()
{
	float tmp;
	DropTarget();
	for( int i = 0; i < g_pGlobals->GetMaxClients(); i++ )
	{
		if( ValidTarget( i ) )
		{
			if( m_mode && m_aimbot )
			{
				tmp = g_pMath->VecDist( LocalEntity.GetOrigin(), EntityList[ i ].GetOrigin() );
				if( tmp < m_Best )
				{
					m_Best = tmp;
					m_Target = i;
				}
			}
			else if( !m_mode && m_aimbot )
			{
				tmp = g_pMath->GetFov( g_pEngine->RetViewAngles(), LocalEntity.GetEyePosition(), EntityList[ i ].GetBonePosition( m_Hitbox ) );
				if( tmp < m_Best )
				{
					m_Best = tmp;
					m_Target = i;
				}
			}

			g_pTrigger->HitBotTriggerBot( i ); // Moved into this thread for performance
		}
	}
}

void Aimbot::GotoTarget()
{
	if( GetAsyncKeyState( 0x01 ) && m_aimbot )
	{
		if( m_Target == -1 )
			return;

		Entity Ent = EntityList[ m_Target ];

		if( Ent.IsValid() && !Ent.IsDead() && Ent.GetTeamNum() != LocalEntity.GetTeamNum() && !LocalEntity.IsActiveWeaponNotVaild() )
		{
			Vector3 AimAngles;
			Vector3 EnemyPos = Ent.GetBonePosition( m_Hitbox );

			if( m_smooth && !m_psilent )
				VelocityComp( EnemyPos, Ent.GetVelocity(), LocalEntity.GetVelocity() );

			Vector3 dir = LocalEntity.GetEyePosition() - EnemyPos;

			g_pMath->VectorNormalize( dir );
			g_pMath->VectorAngles( dir, AimAngles );

			g_pMath->ClampAngle( AimAngles );

			AimAngles -= LocalEntity.GetPunchAngles() * m_PunchAmount;

			g_pMath->ClampAngle( AimAngles );

			if( m_smooth && !m_psilent )
				g_pMath->SmoothAngle( g_pEngine->RetViewAngles(), AimAngles, m_SmoothAmount );

			if( !m_psilent )
				g_pEngine->SetViewAngles( AimAngles );

			if( m_psilent )
				g_pEngine->SetViewAnglesPSilent( AimAngles );
		}
	}
}

bool Aimbot::ValidTarget( int index )
{
	if( !EntityList[ index ].IsValid() )
		return false;

	if( EntityList[ index ].GetTeamNum() == LocalEntity.GetTeamNum() )
		return false;

	if( !EntityList[ index ].IsVisible() )
		return false;

	if( g_pMath->GetFov( g_pEngine->RetViewAngles(), LocalEntity.GetEyePosition(), EntityList[ index ].GetBonePosition( m_Hitbox ) ) > m_Fov )
		return false;

	return true;
}

void Aimbot::DropTarget( void )
{
	m_Target = -1;
	m_Best = 9999.0f;
}

void Aimbot::VelocityComp( Vector3& EnemyPos, Vector3 EnemyVecVelocity, Vector3 PlayerVecVelocity )
{
	EnemyPos += EnemyVecVelocity  * 0.15f;
	EnemyPos -= PlayerVecVelocity * 0.15f;
}

void Aimbot::AlwaysOnRCS()
{
	if( !m_alwaysonrcs || LocalEntity.IsActiveWeaponNotVaild() || LocalEntity.IsActiveWeaponPistol() )
		return;

	Vector3 RCSViewAngle;

	if( !m_rcson )
		m_viewAngle = g_pEngine->RetViewAngles();

	if( GetAsyncKeyState( 0x01 ) )
	{
		RCSViewAngle = m_viewAngle - LocalEntity.GetPunchAngles() * m_PunchAmount;

		g_pMath->ClampAngle( RCSViewAngle );
		g_pEngine->SetViewAngles( RCSViewAngle );

		m_rcson = true;
		return;
	}

	m_rcson = false;
}

void Aimbot::SetAimbot()
{
	m_aimbot = !m_aimbot;
	if( m_aimbot )
		m_alwaysonrcs = false;
}

bool Aimbot::GetAimbot()
{
	return m_aimbot;
}

void  Aimbot::SetAlwaysOnRCS()
{
	m_alwaysonrcs = !m_alwaysonrcs;
	if( m_alwaysonrcs )
		m_aimbot = false;
}

bool Aimbot::GetAlwaysOnRCS()
{
	return m_alwaysonrcs;
}

void Aimbot::SetMode()
{
	m_mode = !m_mode;
}

bool Aimbot::GetMode()
{
	return m_mode;
}

void Aimbot::SetPSilent()
{
	m_psilent = !m_psilent;
}

bool Aimbot::GetPSilent()
{
	return m_psilent;
}

void Aimbot::SetSmooth( float smooth )
{
	m_SmoothAmount = smooth;
}

void Aimbot::SetToggleSmooth()
{
	m_smooth = !m_smooth;
}

bool Aimbot::GetToggleSmooth()
{
	return m_smooth;
}

float Aimbot::GetSmooth()
{
	return m_SmoothAmount;
}

void  Aimbot::SetFov( float fov )
{
	m_Fov = fov;
}
float Aimbot::GetFov()
{
	return m_Fov;
}

void Aimbot::SetHitbox( int bone )
{
	m_Hitbox = bone;
}
int  Aimbot::GetHitbox()
{
	return m_Hitbox;
}

void Aimbot::SetRage()
{
	m_rage = !m_rage;

	if( m_rage )
	{
		m_Fov = 360.f;
		m_smooth = false;
	}
	else
	{
		m_Fov = 3.f;
		m_smooth = true;
	}
}

bool Aimbot::GetRage()
{
	return m_rage;
}