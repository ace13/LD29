#include "Player.hpp"
#include "../Util/Resources.hpp"
#include "../InputSystem.hpp"
#include "QuadTree.hpp"
#include "Ground.hpp"
#include "World.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Player::Player(InputSystem& sys) : mInp(sys), mOnGround(false), mFallSpeed(0)
{
	mSheet = Resources::SpriteSheets["player.png"];
}

Player::~Player()
{

}

void Player::setPosition(const sf::Vector2f& pos)
{
	mPosition = pos;
}

sf::Vector2f Player::getPosition() const
{
	return mPosition;
}

void Player::update(double dt)
{
	sf::Vector2f moveSpeed(mInp["Right"].curValue() - mInp["Left"].curValue(), 0); // mInp["Down"].curValue() - mInp["Up"].curValue());

	mPosition += moveSpeed * (float)(dt * 120);
	
	if (mInp["Up"].curValue() > 0.5 && mOnGround)
	{
		mFallSpeed = -5;
		mOnGround = false;
	}

	mPosition.y += mFallSpeed * 30 * dt;
	
	if (mPosition.x < -WORLD_HALFWIDTH_PIXELS + 15)
		mPosition.x = -WORLD_HALFWIDTH_PIXELS + 15;
	else if (mPosition.x > WORLD_HALFWIDTH_PIXELS - 15)
		mPosition.x = WORLD_HALFWIDTH_PIXELS - 15;
	if (mPosition.y < -WORLD_HALFHEIGHT_PIXELS + 15)
		mPosition.y = -WORLD_HALFHEIGHT_PIXELS + 15;
	else if (mPosition.y > WORLD_HALFHEIGHT_PIXELS - 15)
		mPosition.y = WORLD_HALFHEIGHT_PIXELS - 15;

	auto test = mQT->getAllActors(sf::FloatRect(mPosition.x - 35, mPosition.y - 35, 70, 70));

	mOnGround = false;
	for (auto act : test)
	{
		if (typeid(*act) == typeid(Ground&))
		{
			if ((act->getPosition().y > mPosition.y) && (act->getPosition().y - mPosition.y) < 30 && !((Ground*)act)->dug())
				mOnGround = true;
		}
	}

	if (!mOnGround)
	{
		mFallSpeed = std::min(mFallSpeed + dt * 9.81, 9.81);
	}
	else
		mFallSpeed = 0;

	if (mInp["Enter"].curValue() > 0.5)
	{
		auto test = mQT->getAllActors(sf::FloatRect(mPosition.x - 35, mPosition.y - 35, 70, 70));

		for (auto act : test)
		{
			if (typeid(*act) == typeid(Ground&))
			{
				((Ground*)act)->dig(dt);
			}
		}
	}
}

void Player::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(0, 0));
	sprite.setOrigin(15, 15);

	sprite.setPosition(mPosition);

	target.draw(sprite);
}