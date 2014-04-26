#include "Player.hpp"
#include "../Util/Resources.hpp"
#include "../InputSystem.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Player::Player(InputSystem& sys) : mInp(sys)
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
	sf::Vector2f moveSpeed(mInp["Right"].curValue() - mInp["Left"].curValue(), mInp["Down"].curValue() - mInp["Up"].curValue());

	mPosition += moveSpeed * (float)(dt * 120);
}

void Player::draw(sf::RenderTarget& target)
{
	sf::Sprite sprite(mSheet.getTexture());
	sprite.setTextureRect(mSheet.getRect(0, 0));
	sprite.setOrigin(15, 15);

	sprite.setPosition(mPosition);

	target.draw(sprite);
}