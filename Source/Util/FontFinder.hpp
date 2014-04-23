#pragma once

#include <SFML/Graphics/Font.hpp>

class FontFinder
{
public:
	static void init();
	static void deinit();

	static sf::Font findDefaultFont();
	static sf::Font findFont(const std::string& familyWildcard);

private:
	FontFinder();
};