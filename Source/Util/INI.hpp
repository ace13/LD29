#pragma once

#include <string>
#include <map>

class INIFile
{
public:
	INIFile();
	~INIFile();

	bool loadFromFile(const std::string& file);
	bool saveToFile(const std::string& file);

	void addCategory(const std::string& category);
	void addSetting(const std::string& category, const std::string& setting, const std::string& value);

	std::string getValue(const std::string& category, const std::string& setting) const;

	bool hasCategory(const std::string& cat) const;
	bool hasKey(const std::string& cat, const std::string& key) const;

private:
	std::map<std::string, std::map<std::string, std::string> > mData;
	std::string mCurrentSection;
};