#include "Functions.h"

click_point p;

bool Functions::adb_click(int x,int y)
{
	string x_pos = NumberToString(x);
	string y_pos = NumberToString(y);
	exec("adb shell input tap " + x_pos + " " + y_pos);
	return true;
}

bool Functions::init() {
	if (!isgetaddress) {
		procPid = getProcess("LdVBoxHeadless.exe");
		pointer = StringToDWORD(GetClipboardText());
		isgetaddress = true;
	}
	return 0;
}

string Functions::exec(string command) {
	char buffer[128];
	string result = "";

	FILE* pipe = _popen(command.c_str(), "r");
	if (!pipe) {
		return "popen failed!";
	}
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);
	return result;
}

template <typename T>
std::string Functions::NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

void Functions::auto_fishing(int iRod)
{
	init();
	string output = exec("adb kill-server && adb start-server && adb devices && adb remount");
	if (output.find("remount succeeded") == string::npos) {
		cout << "[!] Error while connecting adb" << endl;
	}
	else {
		cout << "[*] Emulator connected" << endl;
		rodState = readMem<int>(pointer + off_rodState);
		if (isRodOpened()) {
			drop_rod();
		}
		else {
			open_playerbag();
			Sleep(1000);
			open_utilitybag();
			Sleep(1000);
			open_fishingrod(iRod);
			Sleep(1000);
			drop_rod();
		}
		while (true) {
			playerState = readMem<int>(pointer + off_playerstate);
			if (isCatchFish()) pull_rod();
			if (isGotFish()) {
				Sleep(60);
				preserve_fish();
				preserve_trash();
			}
			if (isRodBroken()) fix_rod(iRod);
			if (isPlayerIdle()) drop_rod();
		}
	}
}

bool Functions::isGotFish() {
	return (playerState == 8) ? true : false;
}

bool Functions::isPlayerIdle() {
	return (playerState == 0) ? true : false;
}

bool Functions::isRodBroken() {
	return (playerState == 10) ? true : false;
}

bool Functions::isRodOpened() {
	return (rodState == 103) ? true : false;
}

bool Functions::isCatchFish() {
	return (playerState == 4) ? true : false;
}

void Functions::drop_rod() {
	adb_click(p.p_bDropFishingRod[0], p.p_bDropFishingRod[1]);
}

void Functions::pull_rod() {
	adb_click(p.p_bPullFishingRod[0], p.p_bPullFishingRod[1]);
}

void Functions::preserve_fish() {
	adb_click(p.p_bPreserveFish[0], p.p_bPreserveFish[1]);
}

void Functions::preserve_trash() {
	adb_click(p.p_bPreserveTrash[0], p.p_bPreserveTrash[1]);
}

void Functions::open_playerbag() {
	adb_click(p.p_bPlayerBag[0], p.p_bPlayerBag[1]);
}

void Functions::open_utilitybag() {
	adb_click(p.p_bUtilityBag[0], p.p_bUtilityBag[1]);
}

void Functions::disable_rod() {
	adb_click(p.p_DisableFishingRod[0], p.p_DisableFishingRod[1]);
}

void Functions::close_playerbag() {
	adb_click(p.p_bClosePlayerBag[0], p.p_bClosePlayerBag[1]);
}

void Functions::fix_rod(int iRod){
	open_playerbag();
	open_utilitybag();
	Sleep(500);
	switch (iRod)
	{
	case 1:
	{
		adb_click(p.p_bFixRod1[0], p.p_bFixRod1[1]);
		break;
	}
		
	case 2:
	{
		adb_click(p.p_bFixRod2[0], p.p_bFixRod2[1]);
		break;
	}
		
	case 3:
	{
		adb_click(p.p_bFixRod3[0], p.p_bFixRod3[1]);
		break;
	}
	case 4:
	{
		adb_click(p.p_bFixRod4[0], p.p_bFixRod4[1]);
		break;
	}
	default:
		break;
	}
	Sleep(1000);
	adb_click(p.p_bPayFixRod[0], p.p_bPayFixRod[1]);
	Sleep(1000);
	adb_click(p.p_bAcceptFixRod[0], p.p_bAcceptFixRod[1]);
	Sleep(1000);
	adb_click(p.p_bClosePlayerBag[0], p.p_bClosePlayerBag[1]);
}


void Functions::open_fishingrod(int iRod) {
	switch (iRod) 
	{
	case 1:
	{
		adb_click(p.p_FishingRod1[0], p.p_FishingRod1[1]);
		break;
	}
	case 2:
	{
		adb_click(p.p_FishingRod2[0], p.p_FishingRod2[1]);
		break;
	}
	case 3:
	{
		adb_click(p.p_FishingRod3[0], p.p_FishingRod3[1]);
		break;
	}
	case 4:
	{
		adb_click(p.p_FishingRod4[0], p.p_FishingRod4[1]);
		break;
	}
	default:
		break;
	}
}

uintptr_t Functions::FindPattern(uintptr_t start, size_t size, char* sig, char* mask)
{
	byte* data = new byte[size];
	if (!ReadProcessMemory(pHandle, (void*)start, data, size, 0))
		return 0;
	for (uintptr_t i = 0; i < size; i++)
		if (DataCompare((byte*)(data + i), (byte*)sig, mask))
			return start + i;
	delete[] data;
	return 0;
}

bool Functions::DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
{
	for (; *pszMask; ++pData, ++pMask, ++pszMask)
	{
		if (*pszMask == '0' && *pData != *pMask)
			return false;
	}
	return (*pszMask == NULL);
}

std::string Functions::GetClipboardText()
{
	OpenClipboard(nullptr);
	HANDLE hData = GetClipboardData(CF_TEXT);

	char* pszText = static_cast<char*>(GlobalLock(hData));
	std::string text(pszText);

	GlobalUnlock(hData);
	CloseClipboard();

	return text;
}

DWORD Functions::StringToDWORD(std::string str)
{
	unsigned int rs;
	std::istringstream ss(&str[2]);
	ss >> std::hex >> rs;
	return rs;
}
