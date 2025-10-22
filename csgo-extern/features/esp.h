#pragma once

class ESP
{
public:

	ESP() : m_weaponnames( { {
			"none",
			"deagle",
			"elite",
			"fiveseven",
			"glock",
			"p228",
			"usp",
			"ak47",
			"aug",
			"awp",
			"famas",
			"g3sg1",
			"galil",
			"galilar",
			"m249",
			"m3",
			"m4a1",
			"mac10",
			"mp5navy",
			"p90",
			"scout",
			"sg550",
			"sg552",
			"tmp",
			"ump45",
			"xm1014",
			"bizon",
			"mag7",
			"negev",
			"sawedoff",
			"tec9",
			"taser",
			"hkp2000",
			"mp7",
			"mp9",
			"nova",
			"p250",
			"scar17",
			"scar20",
			"sg556",
			"ssg08",
			"knifegg",
			"knife",
			"flashbang",
			"hegrenade",
			"smokegrenade",
			"molotov",
			"decoy",
			"incgrenade",
			"c4"
		} } ),
		m_ranknames( { {
				"", // no need to show unranked
				"Shitter I",
				"Shitter II",
				"Shitter III",
				"Shitter IV",
				"Shitter Elite",
				"Shitter Elite Master",
				"Gold Nova I",
				"Gold Nova II",
				"Gold Nova III",
				"Gold Nova Master",
				"Master Guardian I",
				"Master Guardian II",
				"Master Guardian Elite",
				"DMG",
				"Legendary Eagle",
				"Legendary Eagle Master",
				"SMFC",
				"N E R D"
			} } )
	{}


				void DrawESP( int index );

				void SetEnemy();
				bool GetEnemy();

				void SetFriendly();
				bool GetFriendly();

				void SetLegit();
				bool GetLegit();

				void SetAnime();
				bool GetAnime();

				void SetSkeleton();
				bool GetSkeleton();

				void SetHealthBar();
				bool GetHealthBar();

				void SetBox();
				bool GetBox();

				void SetPlayerName();
				bool GetPlayerName();

				void SetRank();
				bool GetRank();

				void SetWeapon();
				bool GetWeapon();

				void SetDistance();
				bool GetDistance();

				void SetAimspot();
				bool GetAimspot();

				void SetHitbox();
				bool GetHitbox();

				bool m_enemy = true;
				bool m_friendly = true;
				bool m_legit = false;

				bool m_skeleton = false;
				bool m_healthbar = true;
				bool m_box = true;
				bool m_playername = true;
				bool m_distance = true;
				bool m_aimspot = true;
				bool m_hitbox = false;
				bool m_weapon = true;
				bool m_ranks = true;
				bool m_anime = true;

private:
	Vector3 hitBoxVectors[ 8 ];
	Matrix3x4 boneMatrix;

	std::array<const char*, 50> m_weaponnames;
	std::array<const char*, 19> m_ranknames;

	HSprite m_animesprite = NULL;
};
extern ESP* g_pESP;