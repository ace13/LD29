#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(uint16_t w, uint16_t h) : mSize(w, h)
{

}

SpriteSheet::SpriteSheet(const sf::Vector2u& size) : mSize(size)
{

}

SpriteSheet::SpriteSheet(const SpriteSheet& rhs) : mTexture(rhs.mTexture), mResolution(rhs.mResolution), mSize(rhs.mSize)
{

}

SpriteSheet::SpriteSheet(SpriteSheet&& rhs) : mTexture(std::move(rhs.mTexture)), mResolution(std::move(rhs.mResolution)), mSize(std::move(rhs.mSize))
{

}

SpriteSheet::~SpriteSheet()
{

}


SpriteSheet& SpriteSheet::operator=(SpriteSheet rhs)
{
	std::swap(mTexture, rhs.mTexture);
	std::swap(mResolution, rhs.mResolution);
	std::swap(mSize, rhs.mSize);
}

bool SpriteSheet::loadFromFile(const std::string& path)
{
	if (!mTexture.loadFromFile(path))
		return false;

	auto size = (sf::Vector2f)mTexture.getSize();
	mResolution = sf::Vector2f(size.x / mSize.x, size.y / mSize.y);

	return true;
}

sf::IntRect SpriteSheet::getRect(uint16_t x, uint16_t y) const
{
	return sf::IntRect(mResolution.x * x, mResolution.y * y, mResolution.x, mResolution.y);
}

const sf::Texture& SpriteSheet::getTexture() const
{
	return mTexture;
}
