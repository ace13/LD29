#include "Player.hpp"
#include "../Util/Resources.hpp"
#include "../InputSystem.hpp"
#include "QuadTree.hpp"
#include "Ground.hpp"
#include "Tree.hpp"
#include "World.hpp"
#include "Building.hpp"
#include "../Util/ShapeDraw.hpp"
#include "../Util/FontFinder.hpp"

#include "Items/MinedOre.hpp"
#include "Items/Wood.hpp"
#include "Items/Sapling.hpp"
#include "Recipes.hpp"
#include "Ladder.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace { sf::Font itemFont = FontFinder::findDefaultFont(); }

Player::Player(InputSystem& sys) : mInp(sys), mInventory(45, 1), mOnGround(false), mFallSpeed(0), mAnim(0), mInBuilding(nullptr)
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
			if (mInBuilding->mCurMenu == Building::Menu_Main)
			{
				mInBuilding->doorOpen();
				mInBuilding->mPlayer = nullptr;
				mInBuilding = nullptr;
			}
			else
			{
				mInBuilding->mCurMenu = Building::Menu_Main;
				mInBuilding->mSelectedInventorySlot = 0;
			}
		}
		else if (mInp["Accept"].pressed())
		{
			switch (mInBuilding->mCurMenu)
			{
			case Building::Menu_Main:
				switch (mInBuilding->mSelectedEntry)
				{
				case 0:
					mInBuilding->doorOpen();
					mInBuilding->mPlayer = nullptr;
					mInBuilding = nullptr;
					return;

				case 1:
					mInBuilding->mCurMenu = Building::Menu_Inventory;
					break;

				case 2:
					mInBuilding->mSelectedInventorySlot = 0;
					mInBuilding->mCurMenu = Building::Menu_Crafting;
					break;
				}
				break;

			case Building::Menu_Inventory:
				if (mInventory.getItem() && !mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot))
				{
					mInBuilding->mInventory.addItem(mInventory.getItem(0));
					mInventory.removeItem(mInventory.getItem(0));
				}
				else if (mInventory.getItem() && mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot))
				{
					auto temp = mInventory.getItem();
					if (temp->getName() == mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot)->getName())
					{
						mInventory.removeItem(temp);
						mInBuilding->mInventory.addItem(temp);
					}
					else
					{
						mInventory.removeItem(temp);
						mInventory.addItem(mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot));
						mInBuilding->mInventory.removeItem(mInventory.getItem());
						mInBuilding->mInventory.addItem(temp);
					}
				}
				else if (mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot) && mInventory.freeSlots() > 0)
				{
					auto item = mInBuilding->mInventory.getItem(mInBuilding->mSelectedInventorySlot);
					mInventory.addItem(item);
					mInBuilding->mInventory.removeItem(item);
				}
				break;

			case Building::Menu_Crafting:
				{
					auto recipes = Recipes::getAllRecipes();
					
					auto selected = recipes[mInBuilding->mSelectedInventorySlot];
					auto costs = Recipes::getRecipeCost(selected);

					bool hasAll = true;
					for (auto& c : costs)
					{
						auto item = mInBuilding->mInventory.getItem(c.first);

						if (!item)
						{
							hasAll = false;
							break;
						}
							
						if (item->getWeight() < c.second)
						{
							hasAll = false;
							break;
						}
					}

					if (hasAll)
					{
						for (auto& c : costs)
						{
							auto item = mInBuilding->mInventory.getItem(c.first);
							item->removeAmount(c.second);
						}

						auto item = Recipes::followRecipe(selected);
						
						auto pInv = mInventory.getItem();
						if (pInv && pInv->getName() == item->getName() && pInv->getWeight() + item->getWeight() <= pInv->maxWeight())
						{
							pInv->addAmount(item->getAmount());
							delete item;
							item = nullptr;
						}		
						else if (!pInv)
						{
							mInventory.addItem(item);
						}
						else
							mInBuilding->mInventory.addItem(item);
					}
				} break;
			}
			
		}

		if (mInp["Up"].pressed())
		{
			if (mInBuilding->mCurMenu == Building::Menu_Main)
			{
				--mInBuilding->mSelectedEntry;

				if (mInBuilding->mSelectedEntry < 0)
				{
					mInBuilding->mSelectedEntry = 5;
					mInBuilding->mMenuAnim += 6;
				}
			}
			else if (mInBuilding->mCurMenu == Building::Menu_Inventory)
			{
				mInBuilding->mSelectedInventorySlot -= 10;
				if (mInBuilding->mSelectedInventorySlot < 0)
				{
					mInBuilding->mSelectedInventorySlot = (mInBuilding->mInventory.totalSlots() + mInBuilding->mSelectedInventorySlot);
				}
			}
			else if (mInBuilding->mCurMenu == Building::Menu_Crafting)
			{
				--mInBuilding->mSelectedInventorySlot;
				if (mInBuilding->mSelectedInventorySlot < 0)
					mInBuilding->mSelectedInventorySlot = 0;
			}
		}
		else if (mInp["Down"].pressed())
		{
			if (mInBuilding->mCurMenu == Building::Menu_Main)
			{
				++mInBuilding->mSelectedEntry;

				if (mInBuilding->mSelectedEntry > 5)
				{
					mInBuilding->mSelectedEntry = 0;
					mInBuilding->mMenuAnim -= 6;
				}
			}
			else if (mInBuilding->mCurMenu == Building::Menu_Inventory)
			{
				mInBuilding->mSelectedInventorySlot += 10;
				if (mInBuilding->mSelectedInventorySlot >= mInBuilding->mInventory.totalSlots())
				{
					mInBuilding->mSelectedInventorySlot = (mInBuilding->mSelectedInventorySlot - mInBuilding->mInventory.totalSlots());
				}
			}
			else if (mInBuilding->mCurMenu == Building::Menu_Crafting)
			{
				auto temp = Recipes::getRecipeCount() - 1;

				++mInBuilding->mSelectedInventorySlot;
				if (mInBuilding->mSelectedInventorySlot > temp)
					mInBuilding->mSelectedInventorySlot = temp;
			}
		}

		if (mInp["Left"].pressed())
		{
			if (mInBuilding->mCurMenu == Building::Menu_Inventory)
			{
				auto row = mInBuilding->mSelectedInventorySlot / 10;
				--mInBuilding->mSelectedInventorySlot;
				if (row != mInBuilding->mSelectedInventorySlot / 10)
				{
					mInBuilding->mSelectedInventorySlot += 10;
				}
			}
		}
		else if (mInp["Right"].pressed())
		{
			if (mInBuilding->mCurMenu == Building::Menu_Inventory)
			{
				auto row = mInBuilding->mSelectedInventorySlot / 10;
				++mInBuilding->mSelectedInventorySlot;
				if (row != mInBuilding->mSelectedInventorySlot / 10)
				{
					mInBuilding->mSelectedInventorySlot -= 10;
				}
			}
		}
	}
	else
	{
		mAnim += dt;

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

		if (mOnLadder)
		{
			mOnGround = true;
			mFallSpeed = 0;

			if (mInp["Dig"].curValue() < 0.5)
			{
				float vertMove = mInp["Down"].curValue() - mInp["Up"].curValue();
				mSpeed.y = vertMove;
			}
			else
				mSpeed.y = 0;
		}
		else
			mSpeed.y = 0;

		mOnLadder = false;
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

						if (intersect.width > 15 && intersect.height > 15)
							mPosition.y -= intersect.height;
					}
					else if (pos.y >= mPosition.y)
					{
						if (std::abs(pos.x - mPosition.x) < 27)
						{
							if (mFallSpeed >= 0)
							{
								mOnGround = true;
								if (mSpeed.y > 0)
									mSpeed.y = 0;
							}
						}
					}
					else if (pos.y < mPosition.y && std::abs(pos.x - mPosition.x) < 27)
					{
						if (mFallSpeed < 0)
							mFallSpeed = 0;
						if (mSpeed.y < 0)
							mSpeed.y = 0;
					}
				}
			}
			if (typeid(*act) == typeid(Ladder&) && !mOnLadder)
			{
				auto pos = act->getPosition();
				sf::FloatRect actRect(pos.x - 15, pos.y - 15, 30, 30);
				
				if (actRect.intersects(rect))
				{
					mOnLadder = true;
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
					Ground* ground = dynamic_cast<Ground*>(act);
					if (ground->dug())
						continue;

					if (ground->hasOre())
					{
						Ore* ore = ground->getOre();
						MinedOre* mined = dynamic_cast<MinedOre*>(mInventory.getItem());
						if (!mined && mInventory.freeSlots() > 0)
						{
							mined = new MinedOre(ore->getType());
							mInventory.addItem(mined);
						}

						if (!mined || mined->getWeight() >= mined->maxWeight())
							continue;

						ore->removeAmount(dt);
						mined->addAmount(dt * 5.f);
					}
					else
					{
						((Ground*)act)->dig(dt);
					}
				}
				else if (typeid(*act) == typeid(Tree&))
				{
					Wood* wood = dynamic_cast<Wood*>(mInventory.getItem("Wood"));
					if (!wood && mInventory.freeSlots() > 0)
					{
						wood = new Wood();
						mInventory.addItem(wood);
					}

					if (!wood || wood->getWeight() >= wood->maxWeight())
						continue;

					((Tree*)act)->chop(dt * 2);
					wood->addAmount(dt * 2);
				}
			}
		}

		if (mInp["Use"].pressed())
		{
			bool found = false;

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

					found = true;
					break;
				}
			}

			if (!found)
			{
				auto item = mInventory.getItem();
				if (item)
				{
					if (typeid(*item) == typeid(Ladder::LadderItem&) && item->getAmount() > 0)
					{
						Ladder* l = new Ladder();
						l->setPosition((sf::Vector2f)((sf::Vector2f)(sf::Vector2i(std::round(mPosition.x / 30.f), std::round(mPosition.y / 30.f))) * 30.f) - sf::Vector2f(15, 15));

						auto found = mQT->getAllActors(sf::FloatRect(l->getPosition().x - 5, l->getPosition().y - 5, 30, 30));

						for (auto act : found)
						{
							if (typeid(*act) == typeid(Ladder&))
							{
								delete l;
								l = nullptr;
								break;
							}
							else if (typeid(*act) == typeid(Ground&))
							{
								Ground* ground = dynamic_cast<Ground*>(act);
								if (!ground->dug())
								{
									delete l;
									l = nullptr;
									break;
								}
							}
						}

						if (l)
						{
							mQT->addActor(l);

							item->removeAmount(1);

							if (item->getAmount() <= 0)
							{
								mInventory.removeItem(item);
								delete item;
							}
						}
					}
					else if (typeid(*item) == typeid(Sapling&) && item->getAmount() > 0)
					{
						Tree* l = new Tree();
						l->setPosition((sf::Vector2f)((sf::Vector2f)(sf::Vector2i(std::round(mPosition.x / 30.f), std::round(mPosition.y / 30.f))) * 30.f) - sf::Vector2f(15, 15));

						auto found = mQT->getAllActors(sf::FloatRect(l->getPosition().x - 5, l->getPosition().y - 5, 30, 30));

						for (auto act : found)
						{
							if (typeid(*act) != typeid(Player&))
							{
								delete l;
								l = nullptr;
								break;
							}
						}

						if (l)
						{
							mQT->addActor(l);

							item->removeAmount(1);

							if (item->getAmount() <= 0)
							{
								mInventory.removeItem(item);
								delete item;
							}
						}
					}
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
		mInBuilding->drawMenu(target, Building::Menu_Main);

		if (mInBuilding->mCurMenu != Building::Menu_Main)
			mInBuilding->drawMenu(target, mInBuilding->mCurMenu);
	}
	else
	{
		sf::Sprite sprite(mSheet.getTexture());
		sprite.setTextureRect(mSheet.getRect(0, (int)mAnim % 2 == 0));

		if (mInp["Dig"].curValue() > 0.5)
		{
			sf::Vector2f digDir(mInp["Right"].curValue() - mInp["Left"].curValue(), mInp["Down"].curValue() - mInp["Up"].curValue());
			if ((digDir.x * digDir.x + digDir.y * digDir.y) > 0.25)
			{
				sprite.setTextureRect(mSheet.getRect(1 + (int)(mAnim*8) % 4, 1));

				if (digDir.x < 0)
					sprite.setScale(-1, 1);
			}
		}
		else if (std::abs(mSpeed.x) > 0.1)
		{
			sprite.setTextureRect(mSheet.getRect(1 + (int)(mAnim*10) % 4, 0));
			if (mSpeed.x < 0)
				sprite.setScale(-1, 1);
		}
		
		sprite.setOrigin(15, 15);

		sprite.setPosition(mPosition);

		target.draw(sprite);

		Shapes::BoundInput inpDraw;
		inpDraw.setScale(0.75, 0.75);
		auto test = mQT->getAllActors(sf::FloatRect(mPosition.x - 35, mPosition.y - 35, 70, 70));
		for (auto act : test)
		{
			if (typeid(*act) == typeid(Building&))
			{
				inpDraw.setPosition(act->getPosition() - sf::Vector2f(7.5, 45));
				inpDraw.setBind(mInp["Use"]);
				target.draw(inpDraw);
			}
			else if (typeid(*act) == typeid(Tree&))
			{
				inpDraw.setPosition(act->getPosition() - sf::Vector2f(7.5, 45));
				inpDraw.setBind(mInp["Dig"]);
				target.draw(inpDraw);
			}
			else if (typeid(*act) == typeid(Ground&) && ((Ground*)act)->hasOre() && !((Ground*)act)->dug())
			{
				inpDraw.setPosition(act->getPosition() - sf::Vector2f(7.5, 45));
				inpDraw.setBind(mInp["Dig"]);
				target.draw(inpDraw);
			}
		}
	}
}

void Player::drawUi(sf::RenderTarget& target)
{
	const float UI_RADIUS = 46;

	sf::CircleShape shape(UI_RADIUS);
	shape.setOrigin(UI_RADIUS, UI_RADIUS);
	shape.setFillColor(sf::Color::Black);

	Shapes::RadialProgressBar prog;
	prog.setBackgroundColor(sf::Color(0, 125, 0));
	prog.setForegroundColor(sf::Color(0, 200, 0));
	prog.setRadius(UI_RADIUS);

	prog.setOrigin(UI_RADIUS + prog.getThickness() / 2.f, UI_RADIUS + prog.getThickness() / 2.f);
	prog.setPosition(UI_RADIUS + 25, target.getView().getSize().y - UI_RADIUS - 25);
	shape.setPosition(UI_RADIUS + 25, target.getView().getSize().y - UI_RADIUS - 25);
	target.draw(shape);

	sf::Text text("", itemFont);
	text.setPosition(UI_RADIUS + 25, target.getView().getSize().y - UI_RADIUS - 25);

	for (uint32_t i = 0; i < mInventory.usedSlots(); ++i)
	{
		auto item = mInventory.getItem(i);

		item->draw(target, sf::Vector2f(UI_RADIUS + 25, target.getView().getSize().y - UI_RADIUS - 25));

		/*
		text.setString(item->getName());
		auto rect = text.getLocalBounds();
		text.setOrigin(rect.width / 2.f, rect.height / 2.f);

		target.draw(text);

		text.move(0, rect.height + 8);
		*/

		prog.setValue(item->getWeight() / item->maxWeight());
	}

	target.draw(prog);
}
