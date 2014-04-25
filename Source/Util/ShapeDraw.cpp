#include "ShapeDraw.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace Shapes;

Arc::Arc() : mStartAngle(0), mEndAngle(360), mRadius(32), mLineThickness(2.f)
{

}

Arc::~Arc()
{

}


void Arc::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	//     {
	//         states.transform *= getTransform();
	//         target.draw(..., states);
	//     }

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

RadialProgressBar::RadialProgressBar() : mMinValue(0), mMaxValue(1), mValue(0), mRadius(32), mThickness(16), mOutlineThickness(1.f), mBackground(sf::Color::Green), mForeground(sf::Color::White), mOutlineColor(sf::Color::Black)
{ 

}

RadialProgressBar::~RadialProgressBar()
{

}

void RadialProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	float perc = (mValue - mMinValue) / mMaxValue;

	Arc arc;
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
	
	sf::CircleShape outline(mRadius - mThickness/2.f);
	
	outline.move(mThickness / 2.f, mThickness / 2.f);

	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineColor(mOutlineColor);
	outline.setOutlineThickness(mOutlineThickness);

	target.draw(outline, states);

	outline.setRadius(mRadius + mThickness / 2.f);
	outline.move(-mThickness, -mThickness);

	target.draw(outline, states);
}

