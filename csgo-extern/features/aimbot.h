#pragma once

class Aimbot
{
public:
	void FindTarget();
	void GotoTarget();
	bool IsAbleToShoot();
	void AlwaysOnRCS();

private:
	bool ValidTarget( int iIndex );
	void DropTarget( void );
	void VelocityComp( Vector3& EnemyPos, Vector3 EnemyVecVelocity, Vector3 PlayerVecVelocity );

public:
	void  SetAimbot();
	bool  GetAimbot();

	void  SetAlwaysOnRCS();
	bool  GetAlwaysOnRCS();

	void  SetMode();
	bool  GetMode();

	void  SetPSilent();
	bool  GetPSilent();

	void  SetSmooth( float smooth );
	float GetSmooth();
	void  SetToggleSmooth();
	bool  GetToggleSmooth();

	void  SetFov( float fov );
	float GetFov();

	void SetHitbox( int bone );
	int  GetHitbox();

	void SetRage();
	bool GetRage();

	std::vector<std::string> bonelist;

	float	m_SmoothAmount = 0.75f;
	int		m_Hitbox = 10;
	float	m_Fov = 4.f;
	float   m_PunchAmount = 2.f;
	bool	m_mode = false;	// default to FOV aimbot, so stop crying ted
	bool    m_rage = false;
	bool	m_psilent = false;
	bool	m_aimbot = true;
	bool	m_alwaysonrcs = false;
	bool    m_smooth = true;

private:
	int		m_Target = -1;
	float	m_Best;

	bool	m_predict = true;

	Vector3 m_viewAngle;
	bool	m_rcson = false;
};
extern Aimbot* g_pAimbot;