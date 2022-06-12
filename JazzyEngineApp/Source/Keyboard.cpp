#include "stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{
	mAutorepeatEnabled = false;
}

Keyboard::~Keyboard()
{
	while (!mKeyBuffer.empty())
	{
		mKeyBuffer.pop();
	}
	while (!mCharBuffer.empty())
	{
		mCharBuffer.pop();
	}
}

// Key event stuff
bool Keyboard::KeyIsPressed(unsigned char keycode) const
{
	return mKeyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
	// Check to see if there is something on the key buffer
	if (mKeyBuffer.size() > 0u)
	{
		// Return the front of queue;
		Keyboard::Event e = mKeyBuffer.front();
		mKeyBuffer.pop();
		return e;
	}
	else
	{
		// Create a default constructed event that is invalid
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const
{
	return mKeyBuffer.empty();
}

void Keyboard::ClearKey()
{
	// Clear the key queue
	mKeyBuffer = std::queue<Event>();
}

// Char event stuff
char Keyboard::ReadChar()
{
	// Check to see if there is anything in the char buffer
	if (mCharBuffer.size() > 0u)
	{
		// Return front of queue
		unsigned char charcode = mCharBuffer.front();
		mCharBuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const
{
	return mCharBuffer.empty();
}

void Keyboard::ClearChar()
{
	// Clears the char queue
	mCharBuffer = std::queue<char>();
}

void Keyboard::Clear()
{
	// Calls clear for both Key and Char queues
	ClearKey();
	ClearChar();
}

// Autorepeat
void Keyboard::EnableAutorepeat()
{
	mAutorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat()
{
	mAutorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const
{
	return mAutorepeatEnabled;
}

//// Private functions for window class
void Keyboard::OnKeyPress(unsigned int keycode)
{
	// Set the keystate for that code to true
	mKeyStates[keycode] = true;
	// Create and push an event onto the key queue with event type press and the keycode
	mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	// Call trimbuffer in case there is oversize
	TrimBuffer(mKeyBuffer);
}

void Keyboard::OnKeyRelease(unsigned int keycode)
{
	// Set keystate for the code to false
	mKeyStates[keycode] = false;
	// Create and push an event onto the key queue with event type of release and the keycode
	mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	// Trimbuffer for oversize
	TrimBuffer(mKeyBuffer);
}

void Keyboard::OnChar(char character)
{
	// push character onto char queue
	mCharBuffer.push(character);
	TrimBuffer(mCharBuffer);
}

void Keyboard::ClearState()
{
	// Resets the keystates
	mKeyStates.reset();
}

// Trim buffer for both Event and Char queues
template <typename T> void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
	// while the buffer size is greater than max size, pop off
	while (buffer.size() > mBufferSize)
	{
		buffer.pop();
	}
}