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
		setSetting("input", "sUp", "KB 73");
	if (!hasSetting("input", "sDown"))
		setSetting("input", "sDown", "KB 74");
	if (!hasSetting("input", "sLeft"))
		setSetting("input", "sLeft", "KB 71");
	if (!hasSetting("input", "sRight"))
		setSetting("input", "sRight", "KB 72");
	if (!hasSetting("input", "sJump"))
		setSetting("input", "sJump", "KB 57");
	if (!hasSetting("input", "sEnter"))
		setSetting("input", "sEnter", "KB 58");
	if (!hasSetting("input", "sExit"))
		setSetting("input", "sExit", "KB 36");
}
void Settings::saveSettings()
{
	sFile.saveToFile("settings.ini");
}

bool Settings::hasSetting(const std::string& section, const std::string& key)
{
	return sFile.hasCategory(section) && sFile.hasKey(section, key);
}