#include "Recipes.hpp"
#include "Item.hpp"
#include <algorithm>

std::unordered_map<std::string, std::pair<std::vector<std::pair<std::string, float>>, std::function<Item*()>>> Recipes::sRecipes;

void Recipes::registerRecipe(const std::function<Item*()>& constructor, const std::string& name, const std::initializer_list<std::pair<std::string, float>>& amounts)
{
	std::vector<std::pair<std::string, float>> cost;
	cost.reserve(amounts.size());

	std::for_each(amounts.begin(), amounts.end(), [&cost](const std::pair<std::string, float>& it){ cost.push_back(it); });

	sRecipes[name] = std::make_pair(cost, constructor);
}

std::vector<std::string> Recipes::getAllRecipes()
{
	std::vector<std::string> ret;
	ret.reserve(sRecipes.size());

	for (auto it = sRecipes.begin(); it != sRecipes.end(); ++it)
		ret.push_back(it->first);

	return ret;
}

std::vector<std::pair<std::string, float>> Recipes::getRecipeCost(const std::string& name)
{
	return sRecipes.at(name).first;
}

Item* Recipes::followRecipe(const std::string& name, Item* old)
{
	if (old)
	{
		auto temp = sRecipes.at(name).second();
		old->addAmount(temp->getAmount());
		delete temp;

		return old;
	}
	else
	{
		return sRecipes.at(name).second();
	}
}

size_t Recipes::getRecipeCount()
{
	return sRecipes.size();
}

#include "Ladder.hpp"
#include "Items/Sapling.hpp"

namespace
{
	class DefaultRecipes
	{
	public:
		DefaultRecipes()
		{
			Recipes::registerRecipe<Ladder::LadderItem>("Ladder", { { "Wood", 2.f } });
			Recipes::registerRecipe<Sapling>("Sapling", { { "Wood", 0.5f } });
		}
	} registerer;
}
