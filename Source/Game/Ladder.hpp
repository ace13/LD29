#pragma once

#include "Actor.hpp"
#include "Item.hpp"
#include <SFML/Graphics/Texture.hpp>

class Ladder : public Actor
{
public:
	class LadderItem : public Item
	{
	public:
		LadderItem();
		
		virtual void addAmount(float amount) { mAmount += amount; }
		virtual void removeAmount(float amount) { mAmount -= amount; }
		virtual float getAmount() const { return mAmount; }

		virtual float getWeight() const { return mAmount; }
		virtual float maxWeight() const { return 15; }
		virtual std::string getName() const { return "Ladder"; }

		virtual void draw(sf::RenderTarget& target, const sf::Vector2f& position);

	private:
		float mAmount;
		sf::Texture mTexture;
	};

	Ladder();
	~Ladder();

	virtual void update(double dt);
	virtual void setPosition(const sf::Vector2f&);
	virtual sf::Vector2f getPosition() const;
	virtual void draw(sf::RenderTarget& target);

private:
	sf::Vector2f mPosition;
	sf::Texture mTexture;
};
