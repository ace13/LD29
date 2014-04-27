#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Texture.hpp>

class OBJFile : public sf::Drawable
{
public:
	OBJFile();
	~OBJFile();

	bool loadFromFile(const std::string& file);

	void update(double dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	double mTime;

	sf::Texture mTexture;
	std::vector<sf::Vector3f> mVerts;
	std::vector<sf::Vector2f> mUV;
	std::vector<sf::Vector3i> mFaces1;
	std::vector<sf::Vector3i> mFaces2;
};