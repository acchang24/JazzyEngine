#include "stdafx.h"
#include "Mouse.h"

Mouse::Mouse()
{
	mPosX = 0;
	mPosY = 0;
	leftIsClicked = false;
	rightIsClicked = false;
	isInWindow = false;
	mWheelDeltaCarry = 0;
}

Mouse::~Mouse()
{
	while (!mBuffer.empty())
	{
		mBuffer.pop();
	}
}

std::pair<int, int> Mouse::GetPos() const
{
	return { mPosX, mPosY };
}

int Mouse::GetPosX() const
{
	return mPosX;
}

int Mouse::GetPosY() const
{
	return mPosY;
}

bool Mouse::IsInWindow() const
{
	return isInWindow;
}

bool Mouse::LeftIsClicked() const
{
	return leftIsClicked;
}

bool Mouse::RightIsClicked() const
{
	return rightIsClicked;
}

Mouse::Event Mouse::Read()
{
	// Check to see if there is stuff in buffer
	if (mBuffer.size() > 0u)
	{
		Mouse::Event e = mBuffer.front();
		mBuffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

bool Mouse::IsEmpty() const
{
	return mBuffer.empty();
}

void Mouse::Clear()
{
	mBuffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y)
{
	// Set to new mouse pos
	mPosX = x;
	mPosY = y;
	// Push new mouse event as move
	mBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	// Trim Buffer incase of overflow
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	// Cursor is in window, create an event
	isInWindow = true;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	// Cursor is out of the window, create an event for that
	isInWindow = false;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnLeftClick(int x, int y)
{
	// Set left click to true and push a left click event
	leftIsClicked = true;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::LClick, *this));
	TrimBuffer();
}

void Mouse::OnLeftRelease(int x, int y)
{
	// Set left click to false and push a left release event
	leftIsClicked = false;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightClick(int x, int y)
{
	// Set right click to true and push a right click event
	rightIsClicked = true;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::RClick, *this));
	TrimBuffer();
}

void Mouse::OnRightRelease(int x, int y)
{
	// Set right click to false and push a right release event
	rightIsClicked = false;
	mBuffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	// Create a mouse wheel up event
	mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	// Create a mouse wheel down event
	mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (mBuffer.size() > mBufferSize)
	{
		mBuffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	mWheelDeltaCarry += delta;
	// Generate event for every 120
	while (mWheelDeltaCarry >= WHEEL_DELTA)
	{
		mWheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (mWheelDeltaCarry <= -WHEEL_DELTA)
	{
		mWheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
