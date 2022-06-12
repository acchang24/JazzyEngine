#pragma once

class Mouse
{
	friend class Window;
public:
	// Mouse Event
	class Event
	{
	public:
		enum class Type
		{
			LClick,
			LRelease,
			RClick,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
		Event()
			: mType(Type::Invalid)
			, leftIsPressed(false)
			, rightIsPressed(false)
			, x(0)
			, y(0)
		{}
		Event(Type type, const Mouse& parent)
			: mType(type)
			, leftIsPressed(parent.leftIsClicked)
			, rightIsPressed(parent.rightIsClicked)
			, x(parent.mPosX)
			, y(parent.mPosY)
		{}
		bool IsValid() const
		{
			return mType != Type::Invalid;
		}
		Type GetType() const
		{
			return mType;
		}
		std::pair<int, int> GetPos() const
		{
			return { x, y };
		}
		int GetX() const
		{
			return x;
		}
		int GetY() const
		{
			return y;
		}
		bool LeftIsPressed() const
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return rightIsPressed;
		}

	private:
		Type mType;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	};

	// Mouse
	Mouse();
	~Mouse();
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool LeftIsClicked() const;
	bool RightIsClicked() const;
	Mouse::Event Read();
	bool IsEmpty() const;
	void Clear();

private:
	void OnMouseMove(int x, int y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftClick(int x, int y);
	void OnLeftRelease(int x, int y);
	void OnRightClick(int x, int y);
	void OnRightRelease(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();
	void OnWheelDelta(int x, int y, int delta);

	// Mouse member variables
	static constexpr unsigned int mBufferSize = 16u;
	int mPosX;
	int mPosY;
	bool leftIsClicked;
	bool rightIsClicked;
	bool isInWindow;
	int mWheelDeltaCarry;
	std::queue<Event> mBuffer;
};

