#include "ShapeDraw.hpp"
#include "Resources.hpp"
#include "FontFinder.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

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

namespace
{
	std::string getKeyName(sf::Keyboard::Key k)
	{
#define K(key) case sf::Keyboard::key: return #key
		switch (k)
		{
			K(A); K(B); K(C); K(D); K(E); K(F); K(G); K(H); K(I); K(J); K(K); K(L); K(M); K(N);
			K(O); K(P); K(Q); K(R); K(S); K(T); K(U); K(V); K(W); K(X); K(Y); K(Z); K(Num0);
			K(Num1); K(Num2); K(Num3); K(Num4); K(Num5); K(Num6); K(Num7); K(Num8); K(Num9);
			K(Escape); K(LControl); K(LShift); K(LAlt); K(LSystem); K(RControl); K(RShift);
			K(RAlt); K(RSystem); K(Menu); K(LBracket); K(RBracket); K(SemiColon); K(Comma);
			K(Period); K(Quote); K(Slash); K(BackSlash); K(Tilde); K(Equal); K(Dash); K(Space);
			K(Return); K(BackSpace); K(Tab); K(PageUp); K(PageDown); K(End); K(Home); K(Insert);
			K(Delete); K(Add); K(Subtract); K(Multiply); K(Divide); K(Left); K(Right); K(Up);
			K(Down); K(Numpad0); K(Numpad1); K(Numpad2); K(Numpad3); K(Numpad4); K(Numpad5);
			K(Numpad6); K(Numpad7); K(Numpad8); K(Numpad9); K(F1); K(F2); K(F3); K(F4); K(F5);
			K(F6); K(F7); K(F8); K(F9); K(F10); K(F11); K(F12); K(F13); K(F14); K(F15); K(Pause);
		default:
			return "";
		}
#undef K
	}

	sf::Font keyFont = FontFinder::findDefaultFont();
}

BoundInput::BoundInput() : mPressed(false)
{
	mSheet = Resources::SpriteSheets["controller.png"];
	
}
BoundInput::~BoundInput()
{

}

void BoundInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mBind.getType() == InputSystem::Bind::Type_Invalid)
		return;

	states.transform *= getTransform();

	sf::Sprite sprite;

	switch (mBind.getType())
	{
	case InputSystem::Bind::Type_JoyButton:
		sprite.setTexture(mSheet.getTexture());
		sprite.setTextureRect(mSheet.getRect(mBind.getData().JoyButton.Button, 0));

		if (mPressed)
			sprite.setColor(sf::Color(128, 128, 128));

		target.draw(sprite, states);
		break;

	case InputSystem::Bind::Type_JoyAxis:
		sprite.setTexture(mSheet.getTexture());
		sprite.move(15, 15);
		sprite.setOrigin(15, 15);
		switch (mBind.getData().JoyAxis.Axis)
		{
		case sf::Joystick::X:
		case sf::Joystick::Y:
		case sf::Joystick::R:
		case sf::Joystick::U:
		case sf::Joystick::PovX:
		case sf::Joystick::PovY:
			if (mPressed)
			{
				sprite.setTextureRect(mSheet.getRect(mBind.getData().JoyAxis.Axis > 4 ? 7 : 2, 1));
				if (mBind.getData().JoyAxis.Axis == sf::Joystick::X || mBind.getData().JoyAxis.Axis == sf::Joystick::U || mBind.getData().JoyAxis.Axis == sf::Joystick::PovX)
				{
					if (mBind.getData().JoyAxis.Negative)
						sprite.setRotation(180);
				}
				else
				{
					if (mBind.getData().JoyAxis.Axis == sf::Joystick::PovY)
					{
						if (mBind.getData().JoyAxis.Negative)
							sprite.setRotation(90);
						else
							sprite.setRotation(270);
					}
					else
					{
						if (mBind.getData().JoyAxis.Negative)
							sprite.setRotation(270);
						else
							sprite.setRotation(90);
					}
				}
			}
			else if (mBind.getData().JoyAxis.Axis > 4)
				sprite.setTextureRect(mSheet.getRect(6, 1));
			else
				sprite.setTextureRect(mSheet.getRect(mBind.getData().JoyAxis.Axis > 1, 1));

			target.draw(sprite, states);
			break;

		case sf::Joystick::Z:
			if (mPressed)
				sprite.setTextureRect(mSheet.getRect(5 , 1));
			else
				sprite.setTextureRect(mSheet.getRect(3 + mBind.getData().JoyAxis.Negative, 1));

			target.draw(sprite, states);
			break;
		}
		break;

	case InputSystem::Bind::Type_Keyboard:
		sf::RectangleShape keyBack(sf::Vector2f(30, 30));
		keyBack.setOutlineThickness(2.f);
		keyBack.setOutlineColor(sf::Color::Black);

		if (mPressed)
		{
			keyBack.setFillColor(sf::Color(75, 75, 75));
		}
		else
		{
			keyBack.setFillColor(sf::Color(128, 128, 128));
		}

		sf::Text key(getKeyName((sf::Keyboard::Key)mBind.getData().Keyboard.Key), keyFont);

		auto rect = key.getLocalBounds();
		keyBack.setSize(sf::Vector2f(rect.width + 4, rect.height + 4));
		keyBack.move(-2, 5);

		target.draw(keyBack, states);

		target.draw(key, states);
		break;
	}
}