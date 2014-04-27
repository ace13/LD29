#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Item;

class Recipes
{
public:
	template<typename T>
	static void registerRecipe(const std::string& name, std::initializer_list<std::pair<std::string, float>> amounts) {
		registerRecipe([]() { return new T(); }, name, amounts);
	}

	static void registerRecipe(const std::function<Item*()>& constructor, const std::string& name, const std::initializer_list<std::pair<std::string, float>>& amounts);
	
	static std::vector<std::string> getAllRecipes();
	static std::vector<std::pair<std::string, float>> getRecipeCost(const std::string& name);
	static Item* followRecipe(const std::string& name);

	static size_t getRecipeCount();

private:
	static std::unordered_map<std::string, std::pair<std::vector<std::pair<std::string, float>>, std::function<Item*()>>> sRecipes;
};