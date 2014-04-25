#include "InputSystem.hpp"

#include <SFML/Window/Event.hpp>

namespace
{
	const float TICKRATE = 1.f / 10.f;
}

InputSystem::InputSystem()
{
	///\TODO Move binds somewhere else, probably
	mBinds["Up"]    = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Up });
	mBinds["Down"]  = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Down });
	mBinds["Left"]  = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Left });
	mBinds["Right"] = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Right });
}

InputSystem::~InputSystem()
{
}

void InputSystem::handleEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::JoystickMoved:
	{
		auto joymove = ev.joystickMove;
		float joyvalue = std::abs(joymove.position);

		std::for_each(mBinds.begin(), mBinds.end(), [&joymove, joyvalue](std::unordered_map<std::string, Bind>::value_type& it)
		{
			auto& bindData = it.second.mBindData.JoyAxis;
			if (it.second.mBindType == Bind::Type_JoyAxis && bindData.JoystickId == joymove.joystickId && bindData.Axis == joymove.axis)
			{
				if ((joymove.position < 0 && bindData.Negative) || (joymove.position > 0 && !bindData.Negative))
				{
					it.second.mLastValue = it.second.mValue;
					it.second.mValue = joyvalue / 100.f;
				}
			}
		});

	} break;

	case sf::Event::JoystickButtonPressed:
	case sf::Event::JoystickButtonReleased:
	{
		auto joybutton = ev.joystickButton;
		bool pressed = ev.type == sf::Event::JoystickButtonPressed;

		std::for_each(mBinds.begin(), mBinds.end(), [&joybutton, pressed](std::unordered_map<std::string, Bind>::value_type& it)
		{
			auto& bindData = it.second.mBindData.JoyButton;
			if (it.second.mBindType == Bind::Type_JoyButton && bindData.JoystickId == joybutton.joystickId && bindData.Button == joybutton.button)
			{
				it.second.mLastValue = it.second.mValue;
				it.second.mValue = pressed;
			}
		});
	} break;

	case sf::Event::KeyPressed:
	case sf::Event::KeyReleased:
	{
		auto key = ev.key;
		bool pressed = ev.type == sf::Event::KeyPressed;

		std::for_each(mBinds.begin(), mBinds.end(), [&key, pressed](std::unordered_map<std::string, Bind>::value_type& it)
		{
			auto& bindData = it.second.mBindData.Keyboard;
			if (it.second.mBindType == Bind::Type_Keyboard && bindData.Key == key.code)
			{
				it.second.mLastValue = it.second.mValue;
				it.second.mValue = pressed;
			}
		});
	} break;
	}
}

void InputSystem::update(float dt)
{
	mCurTick += dt;

	while (mCurTick > TICKRATE)
	{
		std::for_each(mBinds.begin(), mBinds.end(), [](std::unordered_map<std::string, Bind>::value_type& it)
		{
			switch (it.second.mBindType)
			{
			case Bind::Type_JoyAxis:
			{
				auto& bindData = it.second.mBindData.JoyAxis;
				auto val = sf::Joystick::getAxisPosition(bindData.JoystickId, (sf::Joystick::Axis)bindData.Axis);

				if ((val < 0 && bindData.Negative) || (val > 0 && !bindData.Negative))
				{
					it.second.mLastValue = it.second.mValue;
					it.second.mValue = std::abs(val) / 100.f;
				}
			} break;

			case Bind::Type_JoyButton:
			{
				auto& bindData = it.second.mBindData.JoyButton;
				auto val = sf::Joystick::isButtonPressed(bindData.JoystickId, bindData.Button);

				it.second.mLastValue = it.second.mValue;
				it.second.mValue = val;
			} break;

			case Bind::Type_Keyboard:
			{
				auto& bindData = it.second.mBindData.Keyboard;
				auto val = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)bindData.Key);

				it.second.mLastValue = it.second.mValue;
				it.second.mValue = val;
			} break;
			}
		});

		mCurTick -= TICKRATE;
	}
}

std::vector<std::string> InputSystem::getValidBinds() const
{
	std::vector<std::string> ret;
	ret.reserve(mBinds.size());

	std::for_each(mBinds.begin(), mBinds.end(), [&ret](const std::pair<std::string, Bind>& b) { ret.push_back(b.first); });

	return ret;
}

InputSystem::Bind InputSystem::operator[](const std::string& bind) const
{
	return mBinds.at(bind);
}

void InputSystem::rebind(const std::string& bind, Bind::Type type, const Bind::BindData& data)
{
	mBinds[bind] = Bind(type, data);
}

InputSystem::Bind::Bind() : mBindType(Type_Invalid), mValue(0), mLastValue(0)
{

}

InputSystem::Bind::Bind(Type type, BindData data) : mBindData(data), mBindType(type), mValue(0), mLastValue(0)
{
}

InputSystem::Bind::Bind(const Bind& rhs) : mBindData(rhs.mBindData), mBindType(rhs.mBindType), mValue(rhs.mValue), mLastValue(rhs.mLastValue)
{

}

InputSystem::Bind::Bind(Bind&& rhs) : mBindData(std::move(rhs.mBindData)), mBindType(std::move(rhs.mBindType)), mValue(std::move(rhs.mValue)), mLastValue(std::move(rhs.mLastValue))
{

}

InputSystem::Bind::~Bind()
{

}

InputSystem::Bind& InputSystem::Bind::operator=(Bind rhs)
{
	std::swap(mBindData, rhs.mBindData);
	std::swap(mBindType, rhs.mBindType);
	std::swap(mValue, rhs.mValue);
	std::swap(mLastValue, rhs.mLastValue);

	return *this;
}