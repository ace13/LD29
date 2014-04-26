#include "World.hpp"
#include "Ground.hpp"

World::World() : mQuadTree(sf::FloatRect(-1000, -1000, 2000, 2000), 6)
{
	generateChunk();
}

World::~World()
{

}

void World::generateChunk()
{
	int numGround = 120;

	sf::Vector2f pos(150, 500);
	//pos.x -= 10 * 30;

	for (int i = 0; i < numGround; ++i)
	{
		auto g = new Ground();

		if (i >= 40)
			g->setType(Ground::Rock);
		else if (i >= 20)
			g->setType(Ground::Dirt);

		if (i > 0 && i % 20 == 0)
		{
			pos.x -= 20 * 30;
			pos.y += 30;
		}

		g->setPosition(pos);
		mQuadTree.addActor(g);

		pos.x += 30;
	}
}

void World::update(double dt)
{

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

	auto act = mQuadTree.getAllActors();
	for (auto i : act)
	{
		((Ground*)i)->draw(target);
	}

	mQuadTree.draw(target);
}
