#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace Shapes
{
	class Arc : public sf::Transformable, public sf::Drawable
	{
	public:
		Arc();
		~Arc();

		inline sf::Color getColor() const { return mColor; }
		inline float getRadius() const { return mRadius; }
		inline float getLineThickness() const { return mLineThickness; }
		inline float getStartAngle() const { return mStartAngle; }
		inline float getEndAngle() const { return mEndAngle; }

		inline void setColor(sf::Color color) { mColor = color; }
		inline void setRadius(float radius) { mRadius = radius; }
		inline void setLineThickness(float thickness) { mLineThickness = thickness; }
		inline void setStartAngle(float startAngle) { mStartAngle = startAngle; }
		inline void setEndAngle(float endAngle) { mEndAngle = endAngle; }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		float mStartAngle, mEndAngle, mRadius, mLineThickness;
		sf::Color mColor;
	};

	class RadialProgressBar : public sf::Transformable, public sf::Drawable
	{
	public:
		RadialProgressBar();
		~RadialProgressBar();

		inline float getMinValue() const { return mMinValue; }
		inline float getMaxValue() const { return mMaxValue; }
		inline float getValue() const { return mValue; }
		inline float getRadius() const { return mRadius; }
		inline float getThickness() const { return mThickness; }
		inline float getOutlineThickness() const { return mOutlineThickness; }
		inline sf::Color getForegroundColor() const { return mForeground; }
		inline sf::Color getBackgroundColor() const { return mBackground; }
		inline sf::Color getOutlineColor() const { return mOutlineColor; }

		inline void setMinValue(float min) { mMinValue = min; }
		inline void setMaxValue(float max) { mMaxValue = max; }
		inline void setValue(float value) { mValue = value; }
		inline void setRadius(float radius) { mRadius = radius; }
		inline void setThickness(float thickness) { mThickness = thickness; }
		inline void setOutlineThickness(float thick) { mOutlineThickness = thick; }
		inline void setForegroundColor(const sf::Color& color) { mForeground = color; }
		inline void setBackgroundColor(const sf::Color& color) { mBackground = color; }
		inline void setOutlineColor(const sf::Color& color) { mOutlineColor = color; }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		float mMinValue, mMaxValue, mValue, mRadius, mThickness, mOutlineThickness;
		sf::Color mBackground, mForeground, mOutlineColor;
	};
}