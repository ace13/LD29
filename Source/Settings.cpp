#include "Settings.hpp"

INIFile Settings::sFile;

void Settings::loadSettings()
{
	sFile.loadFromFile("settings.ini");

	// Default settings
	if (!hasSetting("volume", "iSound"))
		setSetting("volume", "iSound", 50);
	if (!hasSetting("volume", "iMusic"))
		setSetting("volume", "iMusic", 50);
	
	if (!hasSetting("input", "sUp"))
		setSetting("input", "sUp", "KB 22");
	if (!hasSetting("input", "sDown"))
		setSetting("input", "sDown", "KB 18");
	if (!hasSetting("input", "sLeft"))
		setSetting("input", "sLeft", "KB 0");
	if (!hasSetting("input", "sRight"))
		setSetting("input", "sRight", "KB 3");
	if (!hasSetting("input", "sUse"))
		setSetting("input", "sUse", "KB 4");
	if (!hasSetting("input", "sJump"))
		setSetting("input", "sJump", "KB 4");
	if (!hasSetting("input", "sDig"))
		setSetting("input", "sDig", "KB 16");
	if (!hasSetting("input", "sAccept"))
		setSetting("input", "sAccept", "KB 58");
	if (!hasSetting("input", "sCancel"))
		setSetting("input", "sCancel", "KB 36");
}
void Settings::saveSettings()
{
	sFile.saveToFile("settings.ini");
}

bool Settings::hasSetting(const std::string& section, const std::string& key)
{
	return sFile.hasCategory(section) && sFile.hasKey(section, key);
}