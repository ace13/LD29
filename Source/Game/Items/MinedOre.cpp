#include "MinedOre.hpp"

MinedOre::MinedOre(Ore::Type t) : mType(t), mAmount(0)
{

}

std::string MinedOre::getName() const
{
	switch (mType)
	{
	case Ore::Iron: return "Iron"; break;
	case Ore::Gold: return "Gold"; break;
	case Ore::Sulfur: return "Sulfur"; break;
	case Ore::Emerald: return "Emerald"; break;
	case Ore::Diamond: return "Diamond"; break;
	}
}