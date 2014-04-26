#include "InputSystem.hpp"
#include "Settings.hpp"

#include <SFML/Window/Event.hpp>
#include <sstream>
#include <cstdint>

namespace
{
	const double TICKRATE = 1.f / 10.f;

	InputSystem::Bind parseBind(const std::string& bindString)
	{
		std::istringstream iss(bindString);
		char tmp[2];
		iss.readsome(tmp, 2);
		std::string type(tmp, 2);

		if (type == "KB")
		{
			uint32_t key;
			iss >> key;

			return InputSystem::Bind(InputSystem::Bind::Type_Keyboard, InputSystem::Bind::BindData::KB{ key });
		}
		else if (type == "JB")
		{
			uint32_t joyId, button;
			iss >> joyId >> button;

			return InputSystem::Bind(InputSystem::Bind::Type_JoyButton, InputSystem::Bind::BindData::JB{ joyId, button });
		}
		else if (type == "JA")
		{
			uint32_t joyId, axis;
			bool negative;
			iss >> joyId >> axis >> negative;

			return InputSystem::Bind(InputSystem::Bind::Type_JoyAxis, InputSystem::Bind::BindData::JA{ joyId, axis, negative });
		}

		throw std::runtime_error("Failed to parse bind");
	}

	std::string serializeBind(const InputSystem::Bind& bind)
	{
		std::ostringstream oss;

		switch (bind.getType())
		{
		case InputSystem::Bind::Type_Keyboard:
			oss << "KB " << bind.getData().Keyboard.Key; break;
		case InputSystem::Bind::Type_JoyAxis:
			oss << "JA " << bind.getData().JoyAxis.JoystickId << " " << bind.getData().JoyAxis.Axis << " " << bind.getData().JoyAxis.Negative; break;
		case InputSystem::Bind::Type_JoyButton:
			oss << "JB " << bind.getData().JoyButton.JoystickId << " " << bind.getData().JoyButton.Button; break;
		}

		return oss.str();
	}
}

InputSystem::InputSystem() : mCurTick(0)
{
	///\TODO Move binds somewhere else, probably
	
	mBinds["Up"]    = parseBind(Settings::getSetting<std::string>("input", "sUp"));
	mBinds["Down"]  = parseBind(Settings::getSetting<std::string>("input", "sDown"));
	mBinds["Left"]  = parseBind(Settings::getSetting<std::string>("input", "sLeft"));
	mBinds["Right"] = parseBind(Settings::getSetting<std::string>("input", "sRight"));

	mBinds["Jump"]  = parseBind(Settings::getSetting<std::string>("input", "sJump"));
	mBinds["Dig"]   = parseBind(Settings::getSetting<std::string>("input", "sDig"));
	mBinds["Use"]   = parseBind(Settings::getSetting<std::string>("input", "sUse"));
	
	mBinds["Accept"] = parseBind(Settings::getSetting<std::string>("input", "sAccept"));
	mBinds["Cancel"] = parseBind(Settings::getSetting<std::string>("input", "sCancel"));


	// Menu binds, these are hardcoded

	mBinds["MenuUp"]    = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Up });
	mBinds["MenuDown"]  = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Down });
	mBinds["MenuLeft"]  = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Left });
	mBinds["MenuRight"] = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Right });
	mBinds["MenuEnter"] = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Return });
	mBinds["MenuExit"]  = Bind(Bind::Type_Keyboard, Bind::BindData::KB{ sf::Keyboard::Escape });
	

	/*
	mBinds["Up"]    = Bind(Bind::Type_JoyAxis, Bind::BindData::JA{ 0, sf::Joystick::Axis::Y, true });
	mBinds["Down"]  = Bind(Bind::Type_JoyAxis, Bind::BindData::JA{ 0, sf::Joystick::Axis::Y, false });
	mBinds["Left"]  = Bind(Bind::Type_JoyAxis, Bind::BindData::JA{ 0, sf::Joystick::Axis::X, true });
	mBinds["Right"] = Bind(Bind::Type_JoyAxis, Bind::BindData::JA{ 0, sf::Joystick::Axis::X, false });
	mBinds["Enter"] = Bind(Bind::Type_JoyButton, Bind::BindData::JB{ 0, 0 });
	mBinds["Exit"]  = Bind(Bind::Type_JoyButton, Bind::BindData::JB{ 0, 1 });
	*/
}

InputSystem::~InputSystem()
{
}

void InputSystem::update(double dt)
{
	//mCurTick += dt;

	//while (mCurTick > TICKRATE)
	{
		std::for_each(mBinds.begin(), mBinds.end(), [](std::map<std::string, Bind>::value_type& it)
		{
			switch (it.second.mBindType)
			{
			case Bind::Type_JoyAxis:
			{
				auto& bindData = it.second.mBindData.JoyAxis;
				auto val = sf::Joystick::getAxisPosition(bindData.JoystickId, (sf::Joystick::Axis)bindData.Axis);

				if ((val <= 0 && bindData.Negative) || (val >= 0 && !bindData.Negative))
				{
					it.second.mLastValue = it.second.mValue;
					it.second.mValue = std::abs(val) / 100.f;

					if (it.second.mValue < JOYSTICK_DEADZONE)
						it.second.mValue = 0;
				}
				else if ((val > 0 && bindData.Negative) || (val < 0 && !bindData.Negative))
				{
					it.second.mLastValue = it.second.mValue;
					it.second.mValue = 0;
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

		//mCurTick -= TICKRATE;
	}
}

std::vector<std::string> InputSystem::getValidBinds() const
{
	std::vector<std::string> ret;
	ret.reserve(mBinds.size());

	std::for_each(mBinds.begin(), mBinds.end(), [&ret](const std::pair<std::string, Bind>& b) { if (b.first.substr(0, 4) != "Menu") ret.push_back(b.first); });

	return ret;
}

InputSystem::Bind InputSystem::operator[](const std::string& bind) const
{
	return mBinds.at(bind);
}

void InputSystem::rebind(const std::string& bind, Bind::Type type, const Bind::BindData& data)
{
	mBinds[bind] = Bind(type, data);
	Settings::setSetting("input", "s" + bind, serializeBind(mBinds[bind]));
	update(1.0f);
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