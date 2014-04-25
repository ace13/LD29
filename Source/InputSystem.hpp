#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace sf { class Event; }

class InputSystem
{
public:
	class Bind
	{
	public:
		enum Type
		{
			Type_Invalid,
			Type_JoyAxis,
			Type_JoyButton,
			Type_Keyboard
			// Type_MouseButton
		};

		union BindData
		{
			struct JA { unsigned int JoystickId, Axis; bool Negative; } JoyAxis;
			struct JB { unsigned int JoystickId, Button; }              JoyButton;
			struct KB { unsigned int Key; }                             Keyboard;
			// struct MB { unsigned int MouseButton; }                     MouseButton;

			BindData() { }
			BindData(JA d) : JoyAxis(d) { }
			BindData(JB d) : JoyButton(d) { }
			BindData(KB d) : Keyboard(d) { }
			// BindData(MB d) : MouseButton(d) { }
		};
		
		Bind();
		Bind(const Bind& rhs);
		Bind(Bind&& rhs);
		~Bind();

		Bind& operator=(Bind);

		inline float curValue() const { return mValue; }
		inline float lastValue() const { return mLastValue; }
		inline float delta() const { return curValue() - lastValue(); }

		inline Type getType() const { return mBindType; }
		inline BindData getData() const { return mBindData; }

	private:
		Bind(Type type, BindData data);

		Type mBindType;
		BindData mBindData;
		float mValue, mLastValue;

		friend class InputSystem;
	};

	InputSystem();
	~InputSystem();

	void handleEvent(const sf::Event& ev);
	void update(float dt);
	
	std::vector<std::string> getValidBinds() const;
	Bind operator[](const std::string&) const;
	void rebind(const std::string& bind, Bind::Type type, const Bind::BindData& data);

private:
	std::unordered_map<std::string, Bind> mBinds;
	float mCurTick;

	// Non-copyable
	InputSystem(const InputSystem&);
	InputSystem(InputSystem&&);
};
