#pragma once

#include "QuadTree.hpp"

const int WORLD_WIDTH = 128;
const int WORLD_HEIGHT = 64;
const int GROUND_LEVEL = 8; // 8 blocks of sky

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
