#pragma once

#include "Util/INI.hpp"
#include <string>
#include <stdexcept>
#include <type_traits>

const std::string CONFIG_PATH = "settings.ini";

class Settings
{
public:
	static void loadSettings();
	static void saveSettings();

	static bool hasSetting(const std::string& section, const std::string& key);
	template<typename T>
	static T getSetting(const std::string& section, const std::string& key)
	{
		return privGetSetting<T>(section, key, std::is_same<T, std::string>());
	}
	template<typename T>
	static void setSetting(const std::string& section, const std::string& key, T value)
	{
		privSetSetting(section, key, value);
	}

private:
	template<typename T>
	struct identity { typedef T type; };

	static void privSetSetting(const std::string& section, const std::string& key, std::string value) { sFile.addSetting(section, key, value); }
	template<typename T>
	static void privSetSetting(const std::string& section, const std::string& key, T value);
	template<typename T>
	static std::string privGetSetting(const std::string& section, const std::string& key, const std::true_type&);
	template<typename T>
	static T privGetSetting(const std::string& section, const std::string& key, const std::false_type&);

	static INIFile sFile;
};

template<typename T>
std::string Settings::privGetSetting(const std::string& section, const std::string& key, const std::true_type&)
{
	return sFile.getValue(section, key);
}

template<typename T>
T Settings::privGetSetting(const std::string& section, const std::string& key, const std::false_type&)
{
	std::string data = sFile.getValue(section, key);
	if (data.empty())
		throw std::runtime_error("No such setting");

	T value;
	sscanf(data.c_str(), (std::string("%") + key[0]).c_str(), &value);

	return value;
}
template<typename T>
void Settings::privSetSetting(const std::string& section, const std::string& key, T value)
{
	char tmp[256];
	sprintf(tmp, (std::string("%") + key[0]).c_str(), value);

	sFile.addSetting(section, key, tmp);
}
