#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf { class RenderTarget; }

class QuadTreeLeaf;
class QuadTree;
class World;

class Actor
{
public:
	Actor() : mQT(nullptr), mQTLeaf(nullptr) { }
	virtual ~Actor() { }

	virtual void update(double dt) = 0;
	virtual sf::Vector2f getPosition() const = 0;
	virtual void draw(sf::RenderTarget& target) = 0;

protected:
	QuadTree* mQT;
	QuadTreeLeaf* mQTLeaf;

	friend class World;
};