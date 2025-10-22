#include "..\include.h"

ESP* g_pESP = new ESP();

char c[ 256 ];

void ESP::DrawESP( int index )
{
	Vector3 vHead = EntityList[ index ].GetBonePosition( 6 );
	Vector3 vAim = EntityList[ index ].GetBonePosition( g_pAimbot->GetHitbox() );

	Color color = Color::White();

	if( EntityList[ index ].GetTeamNum() != LocalEntity.GetTeamNum() )
	{
		if( !m_enemy )
			return;

		if( m_legit && !EntityList[ index ].IsVisible() ) // legit esp
			if( !EntityList[ index ].IsSpotted() )
				return;

		if( EntityList[ index ].IsVisible() ) // If Aimbot hitbox is visible return true
			color = color = Color( 233, 62, 51 );
		else
			color = Color::Yellow();

		m_animesprite = hSprite1;
	}

	else if( EntityList[ index ].GetTeamNum() == LocalEntity.GetTeamNum() )
	{
		if( !m_friendly || m_legit )
			return;

		if( EntityList[ index ].IsVisible() )
			color = Color::Cyan();
		else
			color = Color::Purple();

		m_animesprite = hSprite2;
	}

	Vector3 vAnime = vHead;
	vAnime += Vector3( 0, 0, 3 );
	vHead += Vector3( 0, 0, 9 );

	Vector3 vFeet;

	if( EntityList[ index ].GetFlags() & FL_DUCKING )
	{
		vFeet = vHead - Vector3( 0, 0, 50 );
	}
	else
	{
		vFeet = vHead - Vector3( 0, 0, 75 );
	}

	Vector3 vTop, vBot, vAim2, vAnimeme;

	if( g_pEngine->WorldToScreen( vHead, vTop ) && g_pEngine->WorldToScreen( vFeet, vBot ) && g_pEngine->WorldToScreen( vAim, vAim2 ) && g_pEngine->WorldToScreen( vAnime, vAnimeme ) )
	{
		float h = vBot.y - vTop.y;
		float w = h / 5.0f;

		if( m_aimspot )
		{
			g_pRenderer->DrawRect( vAim2.x - 1, vAim2.y - 1, 2, 2, Color::Green() );
		}

		if( m_playername )
		{
			sprintf( c, "%s", EntityList[ index ].GetName() );
			g_pRenderer->DrawText( hFont, vTop.x, vTop.y - 12, FONT_RENDER_CENTER_H, Color::White(), c );
		}

		if( m_ranks )
		{
			sprintf( c, "%s", m_ranknames[ EntityList[ index ].GetMatchMakingRank() ] );
			g_pRenderer->DrawText( hFont, vTop.x, ( vTop.y - 12 ) - ( m_playername ? 10 : 0 ), FONT_RENDER_CENTER_H, Color::White(), c );
		}

		if( m_weapon )
		{
			sprintf( c, "%s", m_weaponnames[ EntityList[ index ].GetActiveWeapon() ] );
			g_pRenderer->DrawText( hFont, vTop.x, vBot.y + 2, FONT_RENDER_CENTER_H, Color::White(), c );
		}

		if( m_distance )
		{
			int dist = g_pMath->VecDist( LocalEntity.GetOrigin(), EntityList[ index ].GetOrigin() ) / 100;
			sprintf( c, "%im", dist );
			g_pRenderer->DrawText( hFont, vTop.x, ( vBot.y + 2 ) + ( m_weapon ? 10 : 0 ), FONT_RENDER_CENTER_H, Color::White(), c );
		}

		if( m_box )
			g_pRenderer->DrawBorderBoxOut( vTop.x - w, vTop.y, w * 2, h, 1, color, Color::Black() );

		if( m_healthbar )
			g_pRenderer->DrawHealthBar( vTop.x - w, vTop.y, h, EntityList[ index ].GetHealth() );

		if( m_skeleton )
			g_pRenderer->DrawSkeletion( EntityList[ index ], color );

		if( m_hitbox )
		{
			for( int i = 0; i < HITBOX_MAX; i++ )
			{
				boneMatrix = EntityList[ index ].GetBoneMatrix( Hitbox[ i ].iBone );
				g_pHitbox->GetHitBoxVectors( &Hitbox[ i ], hitBoxVectors, boneMatrix );
				g_pRenderer->DrawHitBox( hitBoxVectors, color );
			}
		}

		if( m_anime )
			g_pRenderer->DrawSprite( m_animesprite, vAnimeme.x, vAnimeme.y, w / 300 ); // divide by texture width
	}
}

void ESP::SetEnemy()
{
	m_enemy = !m_enemy;
}

bool ESP::GetEnemy()
{
	return m_enemy;
}

void ESP::SetFriendly()
{
	m_friendly = !m_friendly;
}

bool ESP::GetFriendly()
{
	return m_friendly;
}

void ESP::SetLegit()
{
	m_legit = !m_legit;
}

bool ESP::GetLegit()
{
	return m_legit;
}

void ESP::SetAnime()
{
	m_anime = !m_anime;
}

bool ESP::GetAnime()
{
	return m_anime;
}

void ESP::SetSkeleton()
{
	m_skeleton = !m_skeleton;
}

bool ESP::GetSkeleton()
{
	return m_skeleton;
}

void ESP::SetHealthBar()
{
	m_healthbar = !m_healthbar;
}

bool ESP::GetHealthBar()
{
	return m_healthbar;
}

void ESP::SetBox()
{
	m_box = !m_box;
}

bool ESP::GetBox()
{
	return m_box;
}

void ESP::SetPlayerName()
{
	m_playername = !m_playername;
}

bool ESP::GetPlayerName()
{
	return m_playername;
}

void ESP::SetRank()
{
	m_ranks = !m_ranks;
}

bool ESP::GetRank()
{
	return m_ranks;
}

void ESP::SetWeapon()
{
	m_weapon = !m_weapon;
}

bool ESP::GetWeapon()
{
	return m_weapon;
}

void ESP::SetDistance()
{
	m_distance = !m_distance;
}

bool ESP::GetDistance()
{
	return m_distance;
}

void ESP::SetAimspot()
{
	m_aimspot = !m_aimspot;
}

bool ESP::GetAimspot()
{
	return m_aimspot;
}

void ESP::SetHitbox()
{
	m_hitbox = !m_hitbox;
}

bool ESP::GetHitbox()
{
	return m_hitbox;
}