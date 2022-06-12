#pragma once
#include <bitset>

class Keyboard
{
	friend class Window;
public:

	// Event class
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
		Event() : mType(Type::Invalid), mCode(0u)
		{}
		Event(Type type, unsigned char code) : mType(type), mCode(code)
		{}
		bool IsPress() const
		{
			return mType == Type::Press;
		}
		bool IsRelease() const
		{
			return mType == Type::Release;
		}
		bool IsValid() const
		{
			return mType == Type::Invalid;
		}
		unsigned char GetCode() const
		{
			return mCode;
		}

	private:
		Type mType;
		unsigned char mCode;
	};

	Keyboard();
	~Keyboard();
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// Key event
	bool KeyIsPressed(unsigned char keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	void ClearKey();
	// Char event (text input)
	char ReadChar();
	bool CharIsEmpty() const;
	void ClearChar();
	// Clears both key and char queues
	void Clear();
	// Autorepeat control
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnabled() const;

private:
	void OnKeyPress(unsigned int keycode);
	void OnKeyRelease(unsigned int keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T> static void TrimBuffer(std::queue<T>& buffer);

	// member variables
	static constexpr unsigned int mKeys = 256u;
	static constexpr unsigned int mBufferSize = 16u;
	bool mAutorepeatEnabled;
	std::bitset<mKeys> mKeyStates;
	std::queue<Event> mKeyBuffer;
	std::queue<char> mCharBuffer;
};

