#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <cstdint>

class SpriteSheet
{
public:
	SpriteSheet(uint16_t w, uint16_t h);
	SpriteSheet(const sf::Vector2u& size);
	SpriteSheet(const SpriteSheet& rhs);
	SpriteSheet(SpriteSheet&& rhs);
	~SpriteSheet();

	SpriteSheet& operator=(SpriteSheet rhs);

	bool loadFromFile(const std::string& path);

	sf::IntRect getRect(uint16_t x, uint16_t y) const;
	inline sf::IntRect getRect(const sf::Vector2u& pos) const { return getRect(pos.x, pos.y); }

	const sf::Texture& getTexture() const;

private:
	sf::Texture mTexture;
	sf::Vector2f mResolution;
	sf::Vector2u mSize;
};
