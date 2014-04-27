#include "Player.hpp"
#include "../Util/Resources.hpp"
#include "../InputSystem.hpp"
#include "QuadTree.hpp"
#include "Ground.hpp"
#include "Tree.hpp"
#include "World.hpp"
#include "Building.hpp"
#include "../Util/ShapeDraw.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Player::Player(InputSystem& sys) : mInp(sys), mOnGround(false), mFallSpeed(0), mInBuilding(nullptr)
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
	if (mInBuilding)
	{
		if (mInp["Cancel"].pressed())
		{
			mInBuilding->mPlayer = nullptr;
			mInBuilding = nullptr;
		}
		else if (mInp["Accept"].pressed())
		{
			
		}

		if (mInp["Up"].pressed())
		{
			--mInBuilding->mSelectedEntry;

			if (mInBuilding->mSelectedEntry < 0)
			{
				mInBuilding->mSelectedEntry = 5;
				mInBuilding->mMenuAnim += 6;
			}
		}
		else if (mInp["Down"].pressed())
		{
			++mInBuilding->mSelectedEntry;

			if (mInBuilding->mSelectedEntry > 5)
			{
				mInBuilding->mSelectedEntry = 0;
				mInBuilding->mMenuAnim -= 6;
			}
		}
	}
	else
	{
		sf::Vector2f moveSpeed;
		if (mInp["Dig"].curValue() < 0.5)
			moveSpeed = sf::Vector2f(mInp["Right"].curValue() - mInp["Left"].curValue(), 0);

		mSpeed.x = mSpeed.x + (moveSpeed.x - mSpeed.x) * dt * 7.5;

		if (mInp["Jump"].curValue() > 0.5 && mOnGround)
		{
			mFallSpeed = -6;
			mOnGround = false;
		}

		mPosition.y += mFallSpeed * 30 * dt;

		auto test = mQT->getAllActors(sf::FloatRect(mPosition.x - 30, mPosition.y - 30, 60, 60));

		if (mFallSpeed >= 0)
			mOnGround = false;

		sf::FloatRect rect(mPosition.x - 15, mPosition.y - 15, 30, 30);

		for (auto act : test)
		{
			if (typeid(*act) == typeid(Ground&))
			{
				bool solid = !((Ground*)act)->dug();

				auto pos = act->getPosition();
				sf::FloatRect actRect(pos.x - 16, pos.y - 16, 32, 32);
				sf::FloatRect intersect;

				if (actRect.intersects(rect, intersect) && solid)
				{
					if (std::abs(pos.y - mPosition.y) < 27)
					{
						if (pos.x < mPosition.x && mSpeed.x < 0)
							mSpeed.x = 0;
						else if (pos.x > mPosition.x && mSpeed.x > 0)
							mSpeed.x = 0;
					}
					else if (pos.y > mPosition.y)
					{
						if (std::abs(pos.x - mPosition.x) < 27 && mFallSpeed >= 0)
							mOnGround = true;

					}
					else if (pos.y < mPosition.y && std::abs(pos.x - mPosition.x) < 27 && mFallSpeed < 0)
						mFallSpeed = 0;
				}
			}
		}

		mPosition += mSpeed * (float)(dt * 120);

		if (!mOnGround)
		{
			mFallSpeed = std::min(mFallSpeed + dt * 15, 9.81 * 30);
		}
		else if (mFallSpeed > 0)
			mFallSpeed = 0;

		if (mInp["Dig"].curValue() > 0.5)
		{
			sf::Vector2f digDir(mInp["Right"].curValue() - mInp["Left"].curValue(), mInp["Down"].curValue() - mInp["Up"].curValue());

			auto test = mQT->getAllActors(sf::FloatRect(mPosition.x - 15 + digDir.x * 25, mPosition.y - 15 + digDir.y * 25, 30, 30));

			for (auto act : test)
			{
				if (typeid(*act) == typeid(Ground&))
				{
					((Ground*)act)->dig(dt);
				}
				else if (typeid(*act) == typeid(Tree&))
				{
					((Tree*)act)->chop();
				}
			}
		}

		if (mInp["Use"].pressed())
		{
			for (auto act : test)
			{
				if (typeid(*act) == typeid(Building&))
				{
					mInBuilding = ((Building*)act);
					mInBuilding->mPlayer = this;
					mInBuilding->mSelectedEntry = 0;
					mInBuilding->mMenuAnim = 0;
					mInBuilding->doorOpen();
					mSpeed = sf::Vector2f();
				}
			}
		}

		// Sanitize position
		if (mPosition.x < -WORLD_HALFWIDTH_PIXELS + 15)
			mPosition.x = -WORLD_HALFWIDTH_PIXELS + 15;
		else if (mPosition.x > WORLD_HALFWIDTH_PIXELS - 15)
			mPosition.x = WORLD_HALFWIDTH_PIXELS - 15;
		if (mPosition.y < -WORLD_HALFHEIGHT_PIXELS + 15)
			mPosition.y = -WORLD_HALFHEIGHT_PIXELS + 15;
		else if (mPosition.y > WORLD_HALFHEIGHT_PIXELS - 15)
			mPosition.y = WORLD_HALFHEIGHT_PIXELS - 15;
	}
}

void Player::draw(sf::RenderTarget& target)
{
	if (mInBuilding)
	{
		mInBuilding->drawMenu(target);
	}
	else
	{
		sf::Sprite sprite(mSheet.getTexture());
		sprite.setTextureRect(mSheet.getRect(0, 0));
		sprite.setOrigin(15, 15);

		sprite.setPosition(mPosition);

		target.draw(sprite);
	}
}