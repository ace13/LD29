#include "World.hpp"
#include "Ground.hpp"
#include "Tree.hpp"
#include "Player.hpp"
#include <random>

World::World() : mQuadTree(sf::FloatRect(-WORLD_HALFWIDTH_PIXELS, -WORLD_HALFHEIGHT_PIXELS, WORLD_WIDTH_PIXELS, WORLD_HEIGHT_PIXELS), 13)
{
	
}

World::~World()
{

}

void World::generateWorld(InputSystem& sys)
{
	auto p = new Player(sys);
	p->setPosition(sf::Vector2f(0, -WORLD_HALFHEIGHT_PIXELS + GROUND_LEVEL * 30 - 20));
	mPlayer = p;

	mQuadTree.addActor(p);

	std::random_device rd;
	std::uniform_int_distribution<int> tree(0, 100);

	for (int x = 0; x < WORLD_WIDTH; ++x)
	{
		for (int y = GROUND_LEVEL; y < WORLD_HEIGHT; ++y)
		{
			sf::Vector2f pos(-WORLD_HALFWIDTH_PIXELS + 15 + x * 30, -WORLD_HALFHEIGHT_PIXELS + 15 + y * 30);

			Ground* g;

			if (y >= GROUND_LEVEL + 8)
				g = new Ground(Ground::Rock);
			else if (y >= GROUND_LEVEL + 1)
				g = new Ground(Ground::Dirt);
			else
				g = new Ground(Ground::Grass);

			g->setPosition(pos);
			mQuadTree.addActor(g);
			g->genOre();
		}

		if (tree(rd) < 15)
		{
			Tree* t = new Tree();
			t->setPosition(sf::Vector2f(-WORLD_HALFWIDTH_PIXELS + 15 + x * 30, -WORLD_HALFHEIGHT_PIXELS + GROUND_LEVEL * 30 - 14));
			mQuadTree.addActor(t);
		}

		// Add tree
	}
	/*
	int numGround = 280;

	sf::Vector2f pos(150, 500);

	for (int i = 0; i < numGround; ++i)
	{
		Ground* g;

		if (i >= 40)
			g = new Ground(Ground::Rock);
		else if (i >= 20)
			g = new Ground(Ground::Dirt);
		else
			g = new Ground(Ground::Grass);

		if (i > 0 && i % 20 == 0)
		{
			pos.x -= 20 * 30;
			pos.y += 30;
		}

		g->setPosition(pos);
		g->mQTLeaf = mQuadTree.addActor(g);
		g->mQT = &mQuadTree;
		g->genOre();
		
		pos.x += 30;
	}
	*/
}

Player* World::getPlayer() const
{
	return mPlayer;
}

void World::update(double dt)
{
	mQuadTree.update(dt);

	auto act = mQuadTree.getAllActors();
	for (auto i : act)
	{
		i->update(dt);
	}
}

void World::draw(sf::RenderTarget& target)
{
	sf::FloatRect viewRect;
	{
		auto vC = target.getView().getCenter(), vS = target.getView().getSize();
		viewRect.left = vC.x - vS.x / 2.f - 15;
		viewRect.top = vC.y - vS.y / 2.f - 15;
		viewRect.width = vS.x + 30;
		viewRect.height = vS.y + 30;
	}

	auto act = mQuadTree.getAllActors(viewRect);
	for (auto i : act)
	{
		if (typeid(*i) != typeid(Player&))
			i->draw(target);
	}

	mPlayer->draw(target);

	//mQuadTree.draw(target);
}
