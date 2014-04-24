#include "FontFinder.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>

namespace
{
	bool wildcmp(const wchar_t* wild, const wchar_t* string)
	{
		while ((*string) && (*wild != '*'))
		{
			if ((*wild != *string) && (*wild != '?'))
				return false;

			wild++;
			string++;
		}

		const wchar_t* cp = NULL, *mp = NULL;

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
	 
	std::string GetSystemFontFile(const std::string& faceName)
	{
		static const LPWSTR fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
		HKEY hKey;
		LONG result;
		std::wstring wsFaceName(faceName.begin(), faceName.end());

		// Open Windows font registry key
		result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
		if (result != ERROR_SUCCESS) {
			return "";
		}

		DWORD maxValueNameSize, maxValueDataSize;
		result = RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
		if (result != ERROR_SUCCESS) {
			return "";
		}

		DWORD valueIndex = 0;
		LPWSTR valueName = new WCHAR[maxValueNameSize];
		LPBYTE valueData = new BYTE[maxValueDataSize];
		DWORD valueNameSize, valueDataSize, valueType;
		std::wstring wsFontFile;

		// Look for a matching font name
		do {

			wsFontFile.clear();
			valueDataSize = maxValueDataSize;
			valueNameSize = maxValueNameSize;

			result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);

			valueIndex++;

			if (result != ERROR_SUCCESS || valueType != REG_SZ) {
				continue;
			}

			std::wstring wsValueName(valueName, valueNameSize);

			// Found a match
			if (wildcmp(wsFaceName.c_str(), wsValueName.c_str())) {

				wsFontFile.assign((LPWSTR)valueData, valueDataSize);
				break;
			}
		} while (result != ERROR_NO_MORE_ITEMS);

		delete[] valueName;
		delete[] valueData;

		RegCloseKey(hKey);

		if (wsFontFile.empty()) {
			return "";
		}

		// Build full font file path
		WCHAR winDir[MAX_PATH];
		GetWindowsDirectory(winDir, MAX_PATH);

		std::wstringstream ss;
		ss << winDir << "\\Fonts\\" << wsFontFile;
		wsFontFile = ss.str();

		return std::string(wsFontFile.begin(), wsFontFile.end());
	}
}

void FontFinder::init()
{

}
void FontFinder::deinit()
{

}

sf::Font FontFinder::findDefaultFont()
{
	return findFont("Arial (TrueType)", "Regular");
}

sf::Font FontFinder::findFont(const std::string& wildcard, const std::string& styleWildcard)
{
	auto ret = sf::Font();
	ret.loadFromFile(GetSystemFontFile(wildcard)); ///\TODO Style choosing, properly.

	return ret;
}