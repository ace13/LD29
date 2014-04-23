#include "FontFinder.hpp"

#include <fontconfig.h>
#include <algorithm>
#include <sstream>

namespace
{
	bool wildcmp(const char* wild, const char* string)
	{
		while ((*string) && (*wild != '*'))
		{
			if ((*wild != *string) && (*wild != '?'))
				return false;

			wild++;
			string++;
		}

		const char* cp = NULL, *mp = NULL;

		while (*string)
		{
			if (*wild == '*')
			{
				if (!*++wild)
					return true;

				mp = wild;
				cp = string + 1;
			}
			else if ((*wild == *string) || (*wild == '?'))
			{
				wild++;
				string++;
			}
			else
			{
				wild = mp;
				string = cp++;
			}
		}

		while (*wild == '*')
			wild++;

		return !*wild;
	}

	std::string getFont(const std::string& wildcard)
	{
		std::string ret;

		FcPattern* pat = FcPatternCreate();
		FcObjectSet* os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_FILE, (char *) 0);
		FcFontSet* fs = FcFontList(config, pat, os);
		printf("Total matching fonts: %d\n", fs->nfont);
		for (int i=0; fs && i < fs->nfont; ++i)
		{
			FcPattern* font = fs->fonts[i];
			FcChar8* file, style, family;
			if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
			    FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
			    FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch)
			{
				std::string tmp(file);
				std::reverse(tmp.begin(), tmp.end());
				if (tmp.substr(0,4) != "ftt.")
					continue;

				if (wildcmp(wildcard.c_str(), family))
					ret = file;
			}
		}

		if (fs) FcFontSetDestroy(fs);
		return ret;
	}

	struct
	{
		FcConfig* config;
	} FontConfigData;
}

void FontFinder::init()
{
	FontConfigData.config = FcInitLoadConfigAndFonts();
}
void FontFinder::deinit()
{
	FcFini();
}

sf::Font FontFinder::findDefaultFont()
{
	return findFont("Arial");
}

sf::Font FontFinder::findFont(const std::string& wildcard)
{
	auto ret = sf::Font();
	ret.loadFromFile(GetSystemFontFile(wildcard));

	return ret;
}