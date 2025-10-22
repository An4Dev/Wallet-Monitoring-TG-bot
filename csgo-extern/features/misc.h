#pragma once

class Misc
{
public:
	void BunnyHop();
	void SetBHop();
	bool GetBHop();

	void AutoStrafe();
	void SetAutoStrafe( int direction );
	void AutoStrafeLeftDirection( int direction );
	void AutoStrafeRightDirection( int direction );
	void AutoStrafeNullDirection();
	int GetAutoStrafe();

	void NoFlash();
	void SetNoFlash();
	bool GetNoFlash();

	void FakeLag();
	void SetFakeLag();
	bool GetFakeLag();

	void NoHands();
	void SetNoHands();
	bool GetNoHands();

	void SkinChanger();
	void SetSkinChanger();
	bool GetSkinChanger();

	void SetSkinID( int skinId );
	int  GetSkinID();

	void Derank();
	void SetDerank();

	void InsultGen();
	void SetInsultGen();

	void IrcChatter();
	void SetIrcChatter();

	void ReadYiff();
	void Yiff();
	void SetYiff();

	bool	m_bhop = true;
	int 	m_autostrafe = 0; // REEEEEEEEE
	bool	m_noflash = false;
	bool	m_fakelag = false;
	bool	m_nohands = false;
	bool	m_skinchanger = false; // keep this on by default, been crashing when disconnecting from a game not sure why yet
	int		m_skinId = 265; // kami = anime :^)
	bool    m_derank = false;
	bool	m_insultgen = false;
	bool	m_ircchatter = false;
	bool	m_yiff = false;

private:
	bool m_strafecheck = false;
	bool m_toggleFakelag = false;

	char m_oldnohands[ 256 ];

	std::vector<std::string> m_yifflines;
	bool m_readyiff = false;

	struct FallbackSkin_t
	{
		int		quality;	   // 0x14F0 m_OriginalOwnerXuidLow
		int		prefab;        // 0x14F4 m_OriginalOwnerXuidHigh
		int		paintkitId;	   // 0x14F8 m_nFallbackPaintKit
		int		seed;		   // 0x14FC m_nFallbackSeed
		float	wear;		   // 0x1500 m_flFallbackWear
		int		stattrack;     // 0x1504 m_nFallbackStatTrak
	} FallbackSkin;
};
extern Misc* g_pMisc;