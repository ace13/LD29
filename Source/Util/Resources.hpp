#pragma once

#include "SpriteSheet.hpp"
#include <unordered_map>
#include <string>

template<typename T>
class ResourceManager
{
public:
	ResourceManager() { }
	~ResourceManager() { }

	T operator[](const std::string& str)
	{
		if (mResources.count(str) == 0)
			loadResource(str);

		return mResources.at(str);
	}

private:
	void loadResource(const std::string& str)
	{
		loadResource(str, (T*)nullptr);
	}

	void loadResource(const std::string& str, SpriteSheet*)
	{
		SpriteSheet sheet(0, 0);
		if (str == "tree.png")
			sheet = SpriteSheet(6, 1);

		if (sheet.loadFromFile(str))
			mResources[str] = sheet;
	}

	std::unordered_map<std::string, T> mResources;
};

namespace Resources
{
	static ResourceManager<SpriteSheet> SpriteSheets;
}