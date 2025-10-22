#pragma once

#define FL_ONGROUND			(1 << 0)
#define FL_DUCKING			(1 << 1)
#define FL_INAIR			256
#define FL_INAIRDUCKING		262

class Entity
{
public:
	Entity();
	~Entity();

	void		Update( int iIndex );

	DWORD_PTR	GetPointer();
	DWORD_PTR	GetLocalPointer();
	DWORD_PTR	GetBoneMatrix();

	Matrix3x4	GetBoneMatrix( int iBone );


	Vector3		GetPunchAngles();
	Vector3     GetEyeAngles();
	Vector3		GetOrigin();
	Vector3		GetVelocity();
	Vector3		GetEyePosition();
	Vector3		GetBonePosition( int iBone );

	int         GetActiveWeapon();
	DWORD_PTR   GetActiveWeaponBase();
	bool		IsActiveWeaponNotVaild();
	bool		IsActiveWeaponPistol();

	int			GetTeamNum();
	int			GetHealth();
	int			GetFlags();
	int			GetCrosshairEnt();

	int			GetMatchMakingRank();
	int			GetMatchMakingWins();

	float		GetFlashDuration();
	void		SetFlashDuration();

	int			GetTickBase();

	bool		IsValid();
	bool		IsPlayer();
	bool		IsDead();
	bool        IsSpottedByMask();
	bool		IsSpotted();
	bool		IsDormant();

	bool		IsVisible();
	void		CheckVisible( int bone );

	const char*	GetName();
	const char* GetSteamID();


	void		Clear();

private:
	int				m_iIndex;

	DWORD_PTR		m_dwPointer;
	DWORD_PTR		m_dwLocalPointer;
	DWORD_PTR		m_dwBoneMatrix;

	Matrix3x4		m_BoneMatrix[ 128 ];

	Vector3			m_vEyeAngles;
	Vector3			m_vOrigin;
	Vector3			m_vEye;
	Vector3			m_vPunch;
	Vector3			m_vVelocity;

	int				m_iTeamNum;
	int				m_iHealth;
	int				m_iFlags;
	int				m_iCrossHairID;
	int				m_hActiveWeapon;
	int				m_iWeaponId;

	int				m_iCompetitiveRanking;
	int				m_iCompetitiveWins;

	float			m_flFlashDuration;
	float			m_flServerTime;

	bool			m_bDead;
	bool			m_bIsDormant;
	bool			m_bVisible;
	bool			m_bSpotted;

	INT64			m_i64SpottedByMask;

	char			m_pName[ 64 ];
	char			m_pSteamID[ 33 ];
};

extern Entity LocalEntity;
extern Entity EntityList[ 65 ];
