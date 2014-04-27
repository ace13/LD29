#pragma once

#include <string>
#include <vector>

class Recipes
{
public:
	static void registerRecipe();
	
	static std::vector<std::string> getAllRecipes();

private:
};