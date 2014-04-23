#include "FontFinder.hpp"

#include <fontconfig.h>
#include <algorithm>
#include <sstream>
#include <array>

namespace
{
	struct
	{
		FcConfig* config;
	} FontConfigData;

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

	std::string getFont(const std::string& wildcard, const std::string& stylecard)
	{
		std::string ret;

		FcPattern* pat = FcPatternCreate();
		FcObjectSet* os = FcObjectSetBuild (FC_FAMILY, FC_STYLE, FC_FILE, (char *) 0);
		FcFontSet* fs = FcFontList(FontConfigData.config, pat, os);

		for (int i=0; fs && i < fs->nfont; ++i)
		{
			FcPattern* font = fs->fonts[i];
			FcChar8 *file, *style, *family;
			if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
			    FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch &&
			    FcPatternGetString(font, FC_STYLE, 0, &style) == FcResultMatch)
			{
				std::string tmp((char*)file);
				std::reverse(tmp.begin(), tmp.end());
				if (tmp.substr(0,4) != "ftt.")
					continue;

				if (wildcmp(wildcard.c_str(), (char*)family) && wildcmp(stylecard.c_str(), (char*)style))
					ret = std::string((char*)file);
			}
		}

		if (fs) FcFontSetDestroy(fs);
		return ret;
	}
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
	static std::array<std::tuple<std::string,std::string>, 3> PossibleFonts {
		std::make_tuple("Arial", "Regular"), std::make_tuple("DejaVu Sans", "Regular"), std::make_tuple("Liberation Sans", "Regular")
	};

	sf::Font found;

	std::find_if(PossibleFonts.begin(), PossibleFonts.end(), [&found](const std::tuple<std::string,std::string>& font) -> bool {
		std::string file = getFont(std::get<0>(font), std::get<1>(font));

		return (!file.empty() && found.loadFromFile(file));
	});

	return found;
}

sf::Font FontFinder::findFont(const std::string& wildcard)
{
	auto ret = sf::Font();
	ret.loadFromFile(getFont(wildcard, "*"));

	return ret;
}