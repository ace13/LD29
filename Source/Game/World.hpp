#pragma once

#include "QuadTree.hpp"

#ifdef _DEBUG
const int WORLD_WIDTH = 32;
const int WORLD_HEIGHT = 32;
#else
const int WORLD_WIDTH = 512;
const int WORLD_HEIGHT = 256;
#endif

const int GROUND_LEVEL = 8; // 8 blocks of sky

const int WORLD_WIDTH_PIXELS = WORLD_WIDTH * 30;
const int WORLD_HEIGHT_PIXELS = WORLD_HEIGHT * 30;
const int WORLD_HALFWIDTH_PIXELS = WORLD_WIDTH * 15;
const int WORLD_HALFHEIGHT_PIXELS = WORLD_HEIGHT * 15;

class Player;
class InputSystem;

class World
{
public:
	World();
	~World();

	void update(double dt);

	void generateWorld(InputSystem&);
	void draw(sf::RenderTarget& target);

	Player* getPlayer() const;

private:
	QuadTree mQuadTree;
	Player* mPlayer;
};
