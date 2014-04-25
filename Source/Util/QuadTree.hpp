#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <cstdint>

class Actor;

class QuadTreeLeaf
{
public:
	const int ACTOR_MAXCOUNT = 10;

	QuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth);
	virtual ~QuadTreeLeaf();

	virtual void addActor(Actor* actor);
	std::vector<Actor*> getAllActors() const;

protected:
	std::vector<Actor*> mActors;
	uint8_t mLevel;
};

class QuadTree
{
public:
	QuadTree(sf::FloatRect bounds, uint8_t maxSplitLevels);
	~QuadTree();

	

private:
	QuadTreeLeaf* mMainLeaf;
	
	sf::FloatRect mBounds;
	uint8_t mMaxLeafs;
};

class SplittableQuadTreeLeaf : public QuadTreeLeaf
{
public:
	SplittableQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth);
	~SplittableQuadTreeLeaf();

private:
	void split();

	QuadTreeLeaf* mNW, mNE, mSW, mSE;
};

class FinalQuadTreeLeaf : public QuadTreeLeaf
{
public:
	FinalQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth);
	~FinalQuadTreeLeaf();

private:

};
