#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "../InputSystem.hpp"
#include "SpriteSheet.hpp"

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

		void setCaching(bool cache);
		inline bool getCaching() const { return mDoTextureCaching; }

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
		void setValue(float value);
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

		bool mDoTextureCaching;
		sf::RenderTexture mTextureCache;
	};

	class BoundInput : public sf::Transformable, public sf::Drawable
	{
	public:
		BoundInput();
		~BoundInput();
		
		inline bool getPressed() const { return mPressed; }
		
		inline void setBind(const InputSystem::Bind& b) { mBind = b; }
		inline void setPressed(bool pressed) { mPressed = pressed; }

		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		SpriteSheet mSheet;
		InputSystem::Bind mBind;
		bool mPressed;
	};
}