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


class VertexFormat
{
public:
    enum class ElementType
    {
        Pos2D,
        Pos3D,
        UV2D,
        Normal,
        Color4,
    };

    class Element
    {
    public:
        Element(ElementType type, size_t offset) : mType(type), mOffset(offset)
        {
        }

        size_t GetOffsetAfter() const
        {
            return mOffset + Size();
        }

        size_t GetOffset() const
        {
            return mOffset;
        }

        size_t Size() const
        {
            return SizeOf(mType);
        }

        static size_t SizeOf(ElementType type)
        {
            switch (type)
            {
            case ElementType::Pos2D:
                return sizeof(Vector2);
            case ElementType:: Pos3D:
                return sizeof(Vector3);
            case ElementType::UV2D :
                return sizeof(Vector2);
            case ElementType::Normal :
                return sizeof(Vector3);
            case ElementType::Color4 :
                return sizeof(Color4);
            }
        }

        ElementType GetType() const
        {
            return mType;
        }

    private:
        ElementType mType;
        size_t mOffset;
    };

    template<ElementType Type>
    const Element& Resolve() const
    {
        for (auto& e : mElements)
        {
            if (e.GetType() == Type)
            {
                return e;
            }
        }
        DbgAssert(false, "Could not resolve element type");
        return mElements.front();
    }

    const Element& ResolveByIndex(size_t i) const
    {
        return mElements[i];
    }

    template<ElementType Type>
    VertexFormat& Append()
    {
        mElements.emplace_back(Type, Size());
        return *this;
    }

    size_t Size() const
    {
        return mElements.empty() ? 0u : mElements.back().GetOffsetAfter();
    }

private:
    std::vector<Element> mElements;
};

class DynamicVertex
{
    friend class VBuffer;
public:
    template<VertexFormat::ElementType Type>
    auto& Attr()
    {
        const auto& element = mFormat.Resolve<Type>();
        auto pAttribute = mData + element.GetOffset();
        if (Type == VertexFormat::ElementType::Pos2D)
        {
            return *reinterpret_cast<Vector2*>(pAttribute);
        }
        else if (Type == VertexFormat::ElementType::Pos3D)
        {
            return *reinterpret_cast<Vector3*>(pAttribute);
        }
        else if (Type == VertexFormat::ElementType::UV2D)
        {
            return *reinterpret_cast<Vector2*>(pAttribute);
        }
        else if (Type == VertexFormat::ElementType::Normal)
        {
            return *reinterpret_cast<Vector3*>(pAttribute);
        }
        else if (Type == VertexFormat::ElementType::Color4)
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
        const auto& element = mFormat.ResolveByIndex(i);
        auto pAttribute = mData + element.GetOffset();
        switch (element.GetType())
        {
        case VertexFormat::ElementType::Pos2D:
            SetAttribute<Vector2>(pAttribute, std::forward<T>(val));
            break;
        case VertexFormat::ElementType::Pos2D:
            SetAttribute<Vector2>(pAttribute, std::forward<T>(val));
            break;
        case VertexFormat::ElementType::Pos3D:
            SetAttribute<Vector3>(pAttribute, std::forward<T>(val));
            break;
        case VertexFormat::ElementType::UV2D:
            SetAttribute<Vector2>(pAttribute, std::forward<T>(val));
            break;
        case VertexFormat::ElementType::Normal:
            SetAttribute<Vector3>(pAttribute, std::forward<T>(val));
            break;
        case VertexFormat::ElementType::Color4:
            SetAttribute<Color4>(pAttribute, std::forward<T>(val));
            break;
        default:
            DbgAssert(false, "Bad element type");
            break;
        }
    }

private:
    DynamicVertex(char* data, const VertexFormat& format) : mData(data), mFormat(format)
    {
    }

    template<typename First, typename ...Rest>
    void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
    {
        SetAttributeByIndex(i, std::forward<First>(first));
        SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
    }
    template<typename Dest, typename Src>
    void SetAttribute(char* pAttribute, Src&& val)
    {
        if (std::is_assignable<Dest, Src>::value)
        {
            *reinterpret_cast<Dest*>(pAttribute) = val;
        }
        else
        {
            DbgAssert(false, "Parameter attribute type mismatch");
        }
    }

    char* mData = nullptr;
    const VertexFormat& mFormat;

};

class VBuffer
{
public:
    VBuffer(VertexFormat format) : mFormat(std::move(format))
    {
    }

    const VertexFormat& GetFormat() const
    {
        return mFormat;
    }
    
    // size of vertices
    size_t Size() const
    {
        return mBuffer.size() / mFormat.Size();
    }

    template<typename ...Params>
    void EmplaceBack(Params&&... params)
    {
        mBuffer.resize(mBuffer.size() + mFormat.Size());
        DynamicVertex vhat = Back();
        Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
    }

    DynamicVertex Back()
    {
        return DynamicVertex(mBuffer.data() + mBuffer.size() - mFormat.Size(), mFormat);
    }

    DynamicVertex Front()
    {
        return DynamicVertex(mBuffer.data(), mFormat);
    }

    DynamicVertex operator[](size_t i)
    {
        return DynamicVertex(mBuffer.data() + mFormat.Size() * i, mFormat);
    }

private:
    std::vector<char> mBuffer;
    VertexFormat mFormat;
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