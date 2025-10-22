#pragma once

class Config
{
public:
	void OnSetup();

	void Save();
	void Load();
};

extern Config* g_pConfig;