#pragma once
#include "EngineMath.h"

class Color4
{
public:
    Color4()
        : r(0), g(0), b(0), a(0)
    {}
    Color4(float _r, float _g, float _b, float _a = 1.0f)
        : r(_r), g(_g), b(_b), a(_a)
    {}
    float r, g, b, a;
};


class VertexLayout
{
public:
	enum class ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Color4D
	};
	class Element
	{
	public:
		Element(ElementType type, size_t offset)
			:
			type(type),
			offset(offset)
		{}
		size_t GetOffsetAfter() const
		{
			return offset + Size();
		}
		size_t GetOffset() const
		{
			return offset;
		}
		size_t Size() const
		{
			return SizeOf(type);
		}
		static constexpr size_t SizeOf(ElementType type)
		{
			switch (type)
			{
			case ElementType::Position2D:
				return sizeof(Vector2);
			case ElementType::Position3D:
				return sizeof(Vector3);
			case ElementType::Texture2D:
				return sizeof(Vector2);
			case ElementType::Normal:
				return sizeof(Vector3);
			case ElementType::Color4D:
				return sizeof(Color4);
			}
			DbgAssert(false, "Invalid element type");
			return 0u;
		}
		ElementType GetType() const
		{
			return type;
		}
	private:
		ElementType type;
		size_t offset;
	};
public:
	template<ElementType Type>
	const Element& Resolve() const
	{
		for (auto& e : elements)
		{
			if (e.GetType() == Type)
			{
				return e;
			}
		}
		DbgAssert(false, "Could not resolve element type");
		return elements.front();
	}
	const Element& ResolveByIndex(size_t i) const
	{
		return elements[i];
	}
	template<ElementType Type>
	VertexLayout& Append()
	{
		elements.emplace_back(Type, Size());
		return *this;
	}
	size_t Size() const
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}

	size_t GetElementCount() const
	{
		return elements.size();
	}

private:
	std::vector<Element> elements;
};

class DynamicVertex
{
	friend class VBuffer;
public:
	DynamicVertex(char* pData, const VertexLayout& layout)
		:
		pData(pData),
		layout(layout)
	{
		if (!pData)
		{
			DbgAssert(false, "Vertex Error")
		}
	}


	template<VertexLayout::ElementType Type>
	auto& Attr()
	{
		const auto& element = layout.Resolve<Type>();
		auto pAttribute = pData + element.GetOffset();
		if constexpr (Type == VertexLayout::ElementType::Position2D)
		{
			return *reinterpret_cast<Vector2*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::ElementType::Position3D)
		{
			return *reinterpret_cast<Vector3*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::ElementType::Texture2D)
		{
			return *reinterpret_cast<Vector2*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::ElementType::Normal)
		{
			return *reinterpret_cast<Vector3*>(pAttribute);
		}
		else if constexpr (Type == VertexLayout::ElementType::Color4D)
		{
			return *reinterpret_cast<Color4*>(pAttribute);
		}
		else
		{
			DbgAssert(false, "Bad element type");
			return *reinterpret_cast<char*>(pAttribute);
		}
	}
	template<typename T>
	void SetAttributeByIndex(size_t i, T&& val)
	{
		const auto& element = layout.ResolveByIndex(i);
		auto pAttribute = pData + element.GetOffset();
		switch (element.GetType())
		{
		case VertexLayout::ElementType::Position2D:
			SetAttribute<Vector2>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::ElementType::Position3D:
			SetAttribute<Vector3>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::ElementType::Texture2D:
			SetAttribute<Vector2>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::ElementType::Normal:
			SetAttribute<Vector3>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::ElementType::Color4D:
			SetAttribute<Color4>(pAttribute, std::forward<T>(val));
			break;
		default:
			DbgAssert(false, "Bad element type");
		}
	}
private:
	
	template<typename First, typename ...Rest>
	// enables parameter pack setting of multiple parameters by element index
	void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
	{
		SetAttributeByIndex(i, std::forward<First>(first));
		SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
	}
	// helper to reduce code duplication in SetAttributeByIndex
	template<typename Dest, typename Src>
	void SetAttribute(char* pAttribute, Src&& val)
	{
		if constexpr (std::is_assignable<Dest, Src>::value)
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			DbgAssert(false, "Parameter attribute type mismatch");
		}
	}
private:
	char* pData = nullptr;
	const VertexLayout& layout;
};

class ConstVertex
{
public:
	ConstVertex(const DynamicVertex& v) : vertex(v)
	{
	}

	template<VertexLayout::ElementType Type>
	const auto& Attr()const
	{
		return const_cast<DynamicVertex&>(vertex).Attr<Type>();
	}

private:
	DynamicVertex vertex;
};

class VBuffer
{
public:
	VBuffer(VertexLayout layout)
		:
		layout(std::move(layout))
	{}

	const char* GetData() const
	{
		return buffer.data();
	}

	const VertexLayout& GetLayout() const
	{
		return layout;
	}
	size_t Size() const
	{
		return buffer.size() / layout.Size();
	}

	size_t SizeBytes() const
	{
		return buffer.size();
	}

	template<typename ...Params>
	void EmplaceBack(Params&&... params)
	{
		if (sizeof...(params) == layout.GetElementCount())
		{
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
	}
	DynamicVertex Back()
	{
		if (buffer.size() != 0u)
		{
			return DynamicVertex(buffer.data() + buffer.size() - layout.Size(), layout);
		}
	}
	DynamicVertex Front()
	{
		if (buffer.size() != 0u)
		{
			return DynamicVertex(buffer.data(), layout);
		}
	}
	DynamicVertex operator[](size_t i)
	{
		if (i < Size())
		{
			return DynamicVertex(buffer.data() + layout.Size() * i, layout);
		}
	}

	ConstVertex Back() const
	{
		return const_cast<VBuffer*>(this)->Back();
	}
	ConstVertex Front() const
	{
		return const_cast<VBuffer*>(this)->Front();
	}
	ConstVertex operator[](size_t i) const
	{
		return const_cast<VBuffer&>(*this)[i];
	}

private:
	std::vector<char> buffer;
	VertexLayout layout;
};






struct Vertex
{
    Vector3 pos;
    Color4 color;
};

struct VertexTexture
{
    Vector3 pos;
    Vector2 uv;
};

struct VertexPosNorm
{
    Vector3 pos;
    Vector3 normal;
};

struct VertexPosNormUV
{
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

struct VertexPosNormColor
{
    Vector3 pos;
    Vector3 normal;
    Color4 color;
};