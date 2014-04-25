#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <vector>
#include <cstdint>

class Actor;

const double QT_TICKRATE = 1.0 / 33.0;

class QuadTreeLeaf
{
public:
	const int ACTOR_MAXCOUNT = 0;

	QuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth, QuadTreeLeaf* parent = nullptr) : mBounds(bounds), mLevel(splitDepth), mParent(parent) { }
	virtual ~QuadTreeLeaf() { }

	virtual void addActor(Actor* actor) = 0;
	virtual void removeActor(Actor* actor) = 0;
	virtual std::vector<Actor*> getAllActors() const = 0;
	virtual std::vector<Actor*> getAllActors(sf::FloatRect bounds) const = 0;

	virtual void update() = 0;

	virtual void testSplit() = 0;

	virtual void draw(sf::RenderTarget& target) const = 0;

protected:
	sf::FloatRect mBounds;
	uint8_t mLevel;
	QuadTreeLeaf* mParent;

	friend class SplittableQuadTreeLeaf;
	friend class FinalQuadTreeLeaf;
};

class QuadTree
{
public:
	QuadTree(sf::FloatRect bounds, uint8_t maxSplitLevels);
	~QuadTree();

	void update(double dt);

	void addActor(Actor* actor);
	void removeActor(Actor* actor);
	std::vector<Actor*> getAllActors() const;
	std::vector<Actor*> getAllActors(sf::FloatRect bounds) const;

	void testSplit();

	void draw(sf::RenderTarget& target) const;

private:
	QuadTreeLeaf* mMainLeaf;
	
	double mTime;
	sf::FloatRect mBounds;
	uint8_t mMaxLeafs;
};

class SplittableQuadTreeLeaf : public QuadTreeLeaf
{
public:
	SplittableQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth, QuadTreeLeaf* parent = nullptr);
	~SplittableQuadTreeLeaf();

	virtual void addActor(Actor* actor);
	virtual void removeActor(Actor* actor);
	virtual std::vector<Actor*> getAllActors() const;
	virtual std::vector<Actor*> getAllActors(sf::FloatRect bounds) const;

	virtual void update();
	virtual void draw(sf::RenderTarget& target) const;

	virtual void testSplit();

private:
	std::vector<Actor*> mActors;
	void split();
	void unsplit();

	bool mHasSplit;
	QuadTreeLeaf *mNW, *mNE, *mSW, *mSE;
};

class FinalQuadTreeLeaf : public QuadTreeLeaf
{
public:
	FinalQuadTreeLeaf(sf::FloatRect bounds, uint8_t splitDepth, QuadTreeLeaf* parent = nullptr);
	~FinalQuadTreeLeaf();

	virtual void addActor(Actor* actor);
	virtual void removeActor(Actor* actor);
	virtual std::vector<Actor*> getAllActors() const;
	virtual std::vector<Actor*> getAllActors(sf::FloatRect bounds) const;

	virtual void update();
	virtual void draw(sf::RenderTarget& target) const;
	virtual void testSplit() {}

private:
	std::vector<Actor*> mActors;
};
