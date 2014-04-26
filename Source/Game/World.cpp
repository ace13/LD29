#include "World.hpp"
#include "Ground.hpp"
#include "Player.hpp"

World::World() : mQuadTree(sf::FloatRect(-1000, -1000, 2000, 2000), 6)
{
	generateChunk();

	auto p = new Player();
	p->setPosition(sf::Vector2f(200, 460));

	mQuadTree.addActor(p);
}

World::~World()
{

}

void World::generateChunk()
{
	int numGround = 280;

	sf::Vector2f pos(150, 500);
	//pos.x -= 10 * 30;

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
}

void World::update(double dt)
{
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
		viewRect.left = vC.x - vS.x / 2.f;
		viewRect.top = vC.y - vS.y / 2.f;
		viewRect.width = vC.x + vS.x / 2.f;
		viewRect.height = vC.y + vS.y / 2.f;
	}

	auto act = mQuadTree.getAllActors(viewRect);
	for (auto i : act)
	{
		i->draw(target);
	}

	mQuadTree.draw(target);
}
