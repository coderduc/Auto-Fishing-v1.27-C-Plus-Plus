#include "Memory.h"
#include "struct.h"

#pragma once
class Functions : Memory
{
public:
	bool adb_click(int x,int y);
	bool init();
	string exec(string command);
	template <typename T>
	std::string NumberToString(T Number);
	void auto_fishing(int iRod);
	bool isGotFish();
	bool isPlayerIdle();
	bool isRodBroken();
	bool isRodOpened();
	bool isCatchFish();
	void drop_rod();
	void pull_rod();
	void preserve_fish();
	void preserve_trash();
	void open_playerbag();
	void open_utilitybag();
	void disable_rod();
	void close_playerbag();
	void fix_rod(int iRod);
	void open_fishingrod(int iRod);
	uintptr_t FindPattern(uintptr_t start, size_t size, char* sig, char* mask);
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask);
	std::string GetClipboardText();
	DWORD StringToDWORD(std::string str);

	bool isgetaddress = false;
	bool is_auto = true;
	DWORD procPid,pointer;
	int playerState,rodState;
};

