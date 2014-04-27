#include "INI.hpp"

#include <stdexcept>
#include <fstream>

INIFile::INIFile()
{

}
INIFile::~INIFile()
{
}

bool INIFile::loadFromFile(const std::string& file)
{
	std::ifstream ifs;

	ifs.open(file);

	if (!ifs.is_open())
		return false;

	mCurrentSection = "global";

	auto skipWhitespace = [](std::string::iterator& it, const std::string::iterator& end) {
		while (it++ != end)
		{
			if (*it != ' ' && *it != '\t' && *it != '\r' && *it != '\n')
				return;
		}
	};

	while (ifs && !ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);
		auto it = line.begin();

		// skipWhitespace(it, line.end());

		if (it == line.end())
			continue;

		switch (*it)
		{
		case ';': // Comment, ignore it
			break;

		case '[':
		{
			++it;
			std::string sectionName;
			while (*it != ']')
				sectionName += *it++;

			mCurrentSection = sectionName;
		}
			break;

		case ' ':
			throw std::runtime_error("Whitespace!!!"); break;

		default:
		{
			std::string valueName;
			while (*it != '=')
				valueName += *it++;
			++it;
			std::string valueValue;
			while (it != line.end())
				valueValue += *it++;

			mData[mCurrentSection][valueName] = valueValue;
		}
			break;
		}
	}

	return true;
}

bool INIFile::saveToFile(const std::string& file)
{
	std::ofstream ofs;

	ofs.open(file);

	if (!ofs.is_open())
		return false;

	int i = 0;
	for (auto& cat : mData)
	{
		if (cat.first != "global")
		{
			if (i++ != 0)
				ofs << std::endl;
			ofs << "[" << cat.first << "]" << std::endl;
		}

		for (auto& val : cat.second)
		{
			ofs << val.first << "=" << val.second << std::endl;
		}
	}

	return true;
}

void INIFile::addSetting(const std::string& category, const std::string& setting, const std::string& value)
{
	mData[category][setting] = value;
}

std::string INIFile::getValue(const std::string& category, const std::string& setting) const
{
	return mData.at(category).at(setting);
}

bool INIFile::hasCategory(const std::string& cat) const
{
	return mData.count(cat) > 0;
}

bool INIFile::hasKey(const std::string& cat, const std::string& key) const
{
	return mData.at(cat).count(key) > 0;
}