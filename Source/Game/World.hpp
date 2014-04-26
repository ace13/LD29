#pragma once

#include "QuadTree.hpp"

class World
{
public:
	World();
	~World();

	void update(double dt);

	void generateChunk();
	void draw(sf::RenderTarget& target);

private:
	QuadTree mQuadTree;
};
