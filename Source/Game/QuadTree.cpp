#include "QuadTree.hpp"
#include "../Game/Actor.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <stdexcept>
#include <algorithm>
#include <iterator>

QuadTree::QuadTree(sf::FloatRect bounds, uint8_t maxSplitLevels) : mMainLeaf(new SplittableQuadTreeLeaf(bounds, maxSplitLevels)), mTime(0), mBounds(bounds), mMaxLeafs(maxSplitLevels)
{
}

QuadTree::~QuadTree()
{
	if (mMainLeaf)
		delete mMainLeaf;
}

void QuadTree::update(double dt)
{
	mTime += dt;

	if (mTime > QT_TICKRATE)
	{
		mMainLeaf->update();

		mTime -= QT_TICKRATE;
	}
}

QuadTreeLeaf* QuadTree::addActor(Actor* actor)
{
	return mMainLeaf->addActor(actor);
}

void QuadTree::removeActor(Actor* actor)
{
	mMainLeaf->removeActor(actor);
}

std::vector<Actor*> QuadTree::getAllActors() const
{
	return mMainLeaf->getAllActors();
}

std::vector<Actor*> QuadTree::getAllActors(sf::FloatRect bounds) const
{
	return mMainLeaf->getAllActors(bounds);
}

void QuadTree::testSplit()
{
	mMainLeaf->testSplit();
}

void QuadTree::draw(sf::RenderTarget& target) const
{
	mMainLeaf->draw(target);
}



/*
	SPLITTABLE
*/

SplittableQuadTreeLeaf::SplittableQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth, QuadTreeLeaf* parent) : QuadTreeLeaf(bounds, splitDepth, parent), mNW(nullptr), mNE(nullptr), mSW(nullptr), mSE(nullptr), mHasSplit(false)
{
	mActors.reserve(QT_ACTOR_MAXCOUNT);
}
SplittableQuadTreeLeaf::~SplittableQuadTreeLeaf()
{
	if (mHasSplit)
	{
		delete mNW;
		delete mNE;
		delete mSW;
		delete mSE;
	}
}

QuadTreeLeaf* SplittableQuadTreeLeaf::addActor(Actor* actor)
{
	auto pos = actor->getPosition();

	if (mBounds.contains(pos))
	{
		if (mHasSplit)
		{
			if (mNW->mBounds.contains(pos))
				return mNW->addActor(actor);
			else if (mNE->mBounds.contains(pos))
				return mNE->addActor(actor);
			else if (mSW->mBounds.contains(pos))
				return mSW->addActor(actor);
			else if (mSE->mBounds.contains(pos))
				return mSE->addActor(actor);
			else
				throw std::runtime_error("Couldn't find a valid spot for an Actor, something's gone horribly wrong.");
		}
		else
		{
			if (mActors.size() < QT_ACTOR_MAXCOUNT)
			{
				mActors.push_back(actor);
				return this;
			}
			else
			{
				split();

				return addActor(actor);
			}
			
		}
	}
	else
		return mParent->addActor(actor);
}

void SplittableQuadTreeLeaf::removeActor(Actor* actor)
{
	if (mHasSplit)
	{
		mNW->removeActor(actor);
		mNE->removeActor(actor);
		mSW->removeActor(actor);
		mSE->removeActor(actor);
	}
	else
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);

		if (it != mActors.end())
			mActors.erase(it);
	}
}

std::vector<Actor*> SplittableQuadTreeLeaf::getAllActors() const
{
	if (mHasSplit)
	{
		std::vector<Actor*> temp;
		auto inserter = std::back_inserter(temp);
		auto actors = mNW->getAllActors();

		std::copy(actors.begin(), actors.end(), inserter);

		actors = mNE->getAllActors();
		std::copy(actors.begin(), actors.end(), inserter);

		actors = mSW->getAllActors();
		std::copy(actors.begin(), actors.end(), inserter);

		actors = mSE->getAllActors();
		std::copy(actors.begin(), actors.end(), inserter);
		return temp;
	}
	else
	{
		return mActors;
	}
}

std::vector<Actor*> SplittableQuadTreeLeaf::getAllActors(sf::FloatRect bounds) const
{
	std::vector<Actor*> temp;
	auto inserter = std::back_inserter(temp);

	if (mHasSplit)
	{
		auto actors = mNW->getAllActors(bounds);
		std::copy(actors.begin(), actors.end(), inserter);

		actors = mNE->getAllActors(bounds);
		std::copy(actors.begin(), actors.end(), inserter);

		actors = mSW->getAllActors(bounds);
		std::copy(actors.begin(), actors.end(), inserter);

		actors = mSE->getAllActors(bounds);
		std::copy(actors.begin(), actors.end(), inserter);
	}
	else
	{
		std::copy_if(mActors.begin(), mActors.end(), inserter, [&bounds](Actor* actor){ return bounds.contains(actor->getPosition()); });
	}

	return temp;
}

void SplittableQuadTreeLeaf::draw(sf::RenderTarget& target) const
{
	if (mHasSplit)
	{
		mNW->draw(target);
		mNE->draw(target);
		mSW->draw(target);
		mSE->draw(target);
	}
	else
	{
		sf::RectangleShape rect(sf::Vector2f(mBounds.width, mBounds.height));
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(1.f);
		rect.setPosition(mBounds.left, mBounds.top);

		target.draw(rect);
	}
}

void SplittableQuadTreeLeaf::update()
{
	if (mHasSplit)
	{
		mNW->update();
		mNE->update();
		mSW->update();
		mSE->update();

		std::vector<Actor*> totalActors = getAllActors();

		if (totalActors.size() > QT_ACTOR_MAXCOUNT)
			return;

		mActors = totalActors;

		unsplit();
	}
	else
	{
		std::vector<Actor*> totalActors = getAllActors();

		std::for_each(totalActors.begin(), totalActors.end(), [this](Actor* act){ 
			if (!mBounds.contains(act->getPosition()))
			{
				removeActor(act);
				mParent->addActor(act);
			}
		});

		totalActors = getAllActors();

		if (totalActors.size() > QT_ACTOR_MAXCOUNT)
			return;

		mActors = totalActors;

		unsplit();
	}
}

void SplittableQuadTreeLeaf::testSplit()
{
	split();

	mNW->testSplit();
	mNE->testSplit();
	mSW->testSplit();
	mSE->testSplit();
}

void SplittableQuadTreeLeaf::split()
{
	auto temp = mActors;
	mActors.clear();

	mHasSplit = true;

	auto nwBound = sf::FloatRect(mBounds.left, mBounds.top, mBounds.width / 2.f, mBounds.height / 2.f);
	auto neBound = sf::FloatRect(mBounds.left + mBounds.width / 2.f, mBounds.top, mBounds.width / 2.f, mBounds.height / 2.f);
	auto swBound = sf::FloatRect(mBounds.left, mBounds.top + mBounds.height / 2.f, mBounds.width / 2.f, mBounds.height / 2.f);
	auto seBound = sf::FloatRect(mBounds.left + mBounds.width / 2.f, mBounds.top + mBounds.height / 2.f, mBounds.width / 2.f, mBounds.height / 2.f);

	if (mLevel == 1)
	{
		mNW = new FinalQuadTreeLeaf(nwBound, 0, this);
		mNE = new FinalQuadTreeLeaf(neBound, 0, this);
		mSW = new FinalQuadTreeLeaf(swBound, 0, this);
		mSE = new FinalQuadTreeLeaf(seBound, 0, this);
	}
	else
	{
		mNW = new SplittableQuadTreeLeaf(nwBound, mLevel - 1, this);
		mNE = new SplittableQuadTreeLeaf(neBound, mLevel - 1, this);
		mSW = new SplittableQuadTreeLeaf(swBound, mLevel - 1, this);
		mSE = new SplittableQuadTreeLeaf(seBound, mLevel - 1, this);
	}

	for (auto act : temp)
		addActor(act);
}

void SplittableQuadTreeLeaf::unsplit()
{
	delete mNW;
	delete mNE;
	delete mSW;
	delete mSE;

	mNW = mNE = mSW = mSE = nullptr;

	mHasSplit = false;
}


/*

	FINAL

*/

FinalQuadTreeLeaf::FinalQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth, QuadTreeLeaf* parent) : QuadTreeLeaf(bounds, splitDepth, parent)
{

}
FinalQuadTreeLeaf::~FinalQuadTreeLeaf()
{

}

QuadTreeLeaf* FinalQuadTreeLeaf::addActor(Actor* actor)
{
	mActors.push_back(actor);
	return this;
}
void FinalQuadTreeLeaf::removeActor(Actor* actor)
{
	auto it = std::find(mActors.begin(), mActors.end(), actor);

	if (it != mActors.end())
		mActors.erase(it);
}
std::vector<Actor*> FinalQuadTreeLeaf::getAllActors() const
{
	return mActors;
}
std::vector<Actor*> FinalQuadTreeLeaf::getAllActors(sf::FloatRect bounds) const
{
	std::vector<Actor*> ret;
	auto insert = std::back_inserter(ret);

	std::copy_if(mActors.begin(), mActors.end(), insert, [&bounds](Actor* actor) { return bounds.contains(actor->getPosition()); });

	return ret;
}

void FinalQuadTreeLeaf::update()
{
	std::vector<Actor*> totalActors = mActors;

	std::for_each(totalActors.begin(), totalActors.end(), [this](Actor* act){
		if (!mBounds.contains(act->getPosition()))
		{
			removeActor(act);
			mParent->addActor(act);
		}
	});
}

void FinalQuadTreeLeaf::draw(sf::RenderTarget& target) const
{
	sf::RectangleShape rect(sf::Vector2f(mBounds.width, mBounds.height));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1.f);
	rect.setPosition(mBounds.left, mBounds.top);
	
	target.draw(rect);
}