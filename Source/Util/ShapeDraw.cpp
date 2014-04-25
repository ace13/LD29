#include "ShapeDraw.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

using namespace Shapes;

Arc::Arc() : mStartAngle(0), mEndAngle(360), mRadius(32), mLineThickness(2.f)
{

}

Arc::~Arc()
{

}


void Arc::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	float aa = 1;
	auto pos = sf::Vector2f(mRadius, mRadius);

	sf::RectangleShape rect(sf::Vector2f(mLineThickness, aa * 2));
	rect.setOrigin(sf::Vector2f(mLineThickness / 2, aa));
	rect.setFillColor(mColor);
	
	for (float i = mStartAngle; i < mEndAngle; i += aa)
	{
		float rad = (3.1415f / 180.f) * i;
		rect.setPosition(pos.x + cos(rad) * mRadius, pos.y + sin(rad) * mRadius);
		rect.setRotation(i);

		target.draw(rect, states);
	}
}

RadialProgressBar::RadialProgressBar() :
	mMinValue(0), mMaxValue(1), mValue(0), mRadius(32), mThickness(16), mOutlineThickness(1.f), mBackground(sf::Color::Green),
	mForeground(sf::Color::White), mOutlineColor(sf::Color::Black), mDoTextureCaching(false)
{ 

}

RadialProgressBar::~RadialProgressBar()
{

}

void RadialProgressBar::setCaching(bool cache)
{
	mDoTextureCaching = cache;

	if (mDoTextureCaching)
	{
		mTextureCache.create(mRadius * 2 + mThickness * 2 + mOutlineThickness * 2, mRadius * 2 + mThickness * 2 + mOutlineThickness * 2);
		mTextureCache.clear(sf::Color::Transparent);

		draw(mTextureCache, sf::RenderStates(sf::BlendMode::BlendAlpha));

		mTextureCache.display();
	}
	else
		mTextureCache.create(1, 1);
}

void RadialProgressBar::setValue(float value)
{
	auto oldVal = mValue;
	mValue = value;

	if (mDoTextureCaching && oldVal != mValue)
	{
		mTextureCache.clear(sf::Color::Transparent);

		draw(mTextureCache, sf::RenderStates(sf::BlendMode::BlendAlpha));

		mTextureCache.display();
	}
}

void RadialProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	bool cached = false;
	if (typeid(target) != typeid(sf::RenderTexture&))
	{
		states.transform *= getTransform();
		cached = true;
	}

	if (mDoTextureCaching && cached)
	{
		sf::Sprite sprite(mTextureCache.getTexture());
		
		target.draw(sprite, states);
	}
	else 
	{
		float perc = (mValue - mMinValue) / mMaxValue;

		Arc arc;

		arc.move(mRadius, mRadius);

		arc.setLineThickness(mThickness);
		arc.setRadius(mRadius);

		arc.setStartAngle(perc * 360);
		arc.setEndAngle(360);
		arc.setColor(mBackground);

		target.draw(arc, states);

		arc.setStartAngle(0);
		arc.setEndAngle(perc * 360);
		arc.setColor(mForeground);

		target.draw(arc, states);

		sf::CircleShape outline(mRadius - mThickness / 2.f);

		outline.move(mThickness / 2.f + mRadius, mThickness / 2.f + mRadius);

		outline.setFillColor(sf::Color::Transparent);
		outline.setOutlineColor(mOutlineColor);
		outline.setOutlineThickness(mOutlineThickness);

		target.draw(outline, states);

		outline.setRadius(mRadius + mThickness / 2.f);
		outline.move(-mThickness, -mThickness);

		target.draw(outline, states);
	}
}

