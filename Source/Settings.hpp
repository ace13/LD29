#pragma once

#include "Util/INI.hpp"
#include <string>

const std::string CONFIG_PATH = "settings.ini";

class Settings
{
public:
	static void loadSettings();
	static void saveSettings();

	static bool hasSetting(const std::string& section, const std::string& key);
	template<typename T>
	static T getSetting(const std::string& section, const std::string& key);
	template<>
	static std::string getSetting<std::string>(const std::string& section, const std::string& key);
	template<typename T>
	static void setSetting(const std::string& section, const std::string& key, T value);
	template<>
	static void setSetting<std::string>(const std::string& section, const std::string& key, std::string value);

private:
	static INIFile sFile;
};

template<>
std::string Settings::getSetting(const std::string& section, const std::string& key)
{
	return sFile.getValue(section, key);
}

template<typename T>
T Settings::getSetting(const std::string& section, const std::string& key)
{
	std::string data = sFile.getValue(section, key);
	if (data.empty())
		throw std::runtime_error("No such setting");

	T value;
	sscanf(data.c_str(), (std::string("%") + key[0]).c_str(), &value);

	return value;
}

template<>
void Settings::setSetting<std::string>(const std::string& section, const std::string& key, std::string value)
{
	sFile.addSetting(section, key, value);
}

template<typename T>
void Settings::setSetting(const std::string& section, const std::string& key, T value)
{
	char tmp[256];
	sprintf(tmp, (std::string("%") + key[0]).c_str(), value);

	sFile.addSetting(section, key, tmp);
}
