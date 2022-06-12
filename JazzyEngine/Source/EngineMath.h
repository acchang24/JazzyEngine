#pragma once
#include <cmath>
#include <smmintrin.h>
#include <cstring>

// SHUFFLER is like shuffle, but has easier to understand indices
#define _MM_SHUFFLER( xi, yi, zi, wi ) _MM_SHUFFLE( wi, zi, yi, xi )

namespace Math
{
    const float Pi = 3.1415926535f;
    const float TwoPi = Pi * 2.0f;
    const float PiOver2 = Pi / 2.0f;

    inline float ToRadians(float degrees)
    {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(float radians)
    {
        return radians * 180.0f / Pi;
    }

    template <typename T>
    T Max(const T& a, const T& b)
    {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b)
    {
        return (a < b ? a : b);
    }

    template <typename T>
    T Clamp(const T& value, const T& lower, const T& upper)
    {
        return Min(upper, Max(lower, value));
    }

    inline float Lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    // Returns true if val is close enough to be considered the zero
    inline bool IsZero(float val, float epsilon = 0.001f)
    {
        if (fabs(val) <= epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Returns true if a and b are close enough to be considered the same value
    inline bool IsCloseEnuf(float a, float b, float epsilon = 0.001f)
    {
        return fabsf(a - b) <= epsilon * Max(Max(1.0f, fabsf(a)), fabsf(b));
    }
}

class Quaternion;

// 2D Vector
class Vector2
{
public:
    float x;
    float y;

    Vector2()
        : x(0.0f)
        , y(0.0f)
    {}

    explicit Vector2(float inX, float inY)
        : x(inX)
        , y(inY)
    {}

    // Set both components in one line
    void Set(float inX, float inY)
    {
        x = inX;
        y = inY;
    }

    // Vector addition (a + b)
    friend Vector2 operator+(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    // Vector +=
    Vector2& operator+=(const Vector2& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }

    // Vector subtraction (a - b)
    friend Vector2 operator-(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    // Vector -=
    Vector2& operator-=(const Vector2& right)
    {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    // Component-wise multiplication
    // (a.x * b.x, ...)
    friend Vector2 operator*(const Vector2& a, const Vector2& b)
    {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    // Scalar multiplication
    friend Vector2 operator*(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar multiplication
    friend Vector2 operator*(float scalar, const Vector2& vec)
    {
        return Vector2(vec.x * scalar, vec.y * scalar);
    }

    // Scalar *=
    Vector2& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Scalar division
    friend Vector2 operator/(const Vector2& vec, float scalar)
    {
        return Vector2(vec.x / scalar, vec.y / scalar);
    }

    // Scalar /=
    Vector2& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Length squared of vector
    float LengthSq() const
    {
        return (x * x + y * y);
    }

    // Length of vector
    float Length() const
    {
        return (sqrtf(LengthSq()));
    }

    // Normalize this vector
    void Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
    }

    // Normalize the provided vector
    static Vector2 Normalize(const Vector2& vec)
    {
        Vector2 temp = vec;
        temp.Normalize();
        return temp;
    }

    // Dot product between two vectors (a dot b)
    static float Dot(const Vector2& a, const Vector2& b)
    {
        return (a.x * b.x + a.y * b.y);
    }

    // Lerp from A to B by f
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
    {
        return Vector2(a + f * (b - a));
    }

    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;
};

// 3D Vector
class Vector3
{
public:
    union
    {
        __m128 mVec;
        struct { float x, y, z; };
    };

    Vector3()
    {
        mVec = _mm_setzero_ps();
    }

    explicit Vector3(float inX, float inY, float inZ)
    {
        mVec = _mm_setr_ps(inX, inY, inZ, 0.0f);
    }

    explicit Vector3(__m128 value)
    {
        mVec = value;
    }

    // Set all three components in one line
    void Set(float inX, float inY, float inZ)
    {
        mVec = _mm_setr_ps(inX, inY, inZ, 0.0f);
    }

    // Vector addition (a + b)
    friend Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(_mm_add_ps(a.mVec, b.mVec));
    }

    // Vector subtraction (a - b)
    friend Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(_mm_sub_ps(a.mVec, b.mVec));
    }

    // Component-wise multiplication
    friend Vector3 operator*(const Vector3& a, const Vector3& b)
    {
        return Vector3(_mm_mul_ps(a.mVec, b.mVec));
    }

    // Scalar multiplication
    friend Vector3 operator*(const Vector3& vec, float scalar)
    {
        return Vector3(_mm_mul_ps(vec.mVec, _mm_set_ps1(scalar)));
    }

    // Scalar multiplication
    friend Vector3 operator*(float scalar, const Vector3& vec)
    {
        return Vector3(_mm_mul_ps(_mm_set_ps1(scalar), vec.mVec));
    }

    // Scalar *=
    Vector3& operator*=(float scalar)
    {
        mVec = _mm_mul_ps(mVec, _mm_set_ps1(scalar));
        return *this;
    }

    // Scalar division
    friend Vector3 operator/(const Vector3& vec, float scalar)
    {
        return Vector3(_mm_div_ps(vec.mVec, _mm_set_ps1(scalar)));
    }

    // Scalar /=
    Vector3& operator/=(float scalar)
    {
        mVec = _mm_div_ps(mVec, _mm_set_ps1(scalar));
        return *this;
    }

    // Vector +=
    Vector3& operator+=(const Vector3& right)
    {
        mVec = _mm_add_ps(mVec, right.mVec);
        return *this;
    }

    // Vector -=
    Vector3& operator-=(const Vector3& right)
    {
        mVec = _mm_sub_ps(mVec, right.mVec);
        return *this;
    }

    // Length squared of vector (as a vector)
    Vector3 LengthSq_v() const
    {
        return Vector3(_mm_mul_ps(mVec, mVec));
    }

    // Length squared of vector
    float LengthSq() const
    {
        Vector3 length = LengthSq_v();
        return length.x + length.y + length.z;
    }

    // Length of vector (as a vector)
    Vector3 Length_v() const
    {
        Vector3 length = LengthSq_v();
        return Vector3(_mm_rsqrt_ps(length.mVec));
    }

    // Length of vector
    float Length() const
    {
        Vector3 length = Length_v();
        return length.x + length.y + length.z;
    }

    // Normalize this vector
    void Normalize()
    {
        // Calculate length squared (data dot data)
        // The mask 0x77 will dot the x, y, and z components,
        // and store it in results x, y, and z.
        __m128 temp = _mm_dp_ps(mVec, mVec, 0x77);
        // stores (1 / length) in x, y, and z
        temp = _mm_rsqrt_ps(temp);
        // multiply all components by (1 / length)
        mVec = _mm_mul_ps(mVec, temp);
    }

    // Normalize the provided vector
    friend Vector3 Normalize(const Vector3& vec)
    {
        __m128 temp = _mm_dp_ps(vec.mVec, vec.mVec, 0x77);
        temp = _mm_rsqrt_ps(temp);
        return Vector3(_mm_mul_ps(vec.mVec, temp));
    }

    // Dot product between two vectors (a dot b) (as a vector)
    friend Vector3 Dot_v(const Vector3& a, const Vector3& b)
    {
        return Vector3(_mm_dp_ps(a.mVec, b.mVec, 0x77));
    }

    // Dot product between two vectors (a dot b)
    friend float Dot(const Vector3& a, const Vector3& b)
    {
        Vector3 dot = Dot_v(a, b);
        return dot.x;
    }

    // Cross product between two vectors (a cross b)
    friend Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        __m128 tempA = _mm_shuffle_ps(a.mVec, a.mVec, _MM_SHUFFLER(1, 2, 0, 0));
        __m128 tempB = _mm_shuffle_ps(b.mVec, b.mVec, _MM_SHUFFLER(2, 0, 1, 0));
        Vector3 lhs = Vector3(tempA) * Vector3(tempB);
        tempA = _mm_shuffle_ps(a.mVec, a.mVec, _MM_SHUFFLER(2, 0, 1, 0));
        tempB = _mm_shuffle_ps(b.mVec, b.mVec, _MM_SHUFFLER(1, 2, 0, 0));
        Vector3 rhs = Vector3(tempA) * Vector3(tempB);
        return lhs - rhs;
    }

    // Lerp from A to B by f
    friend Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
    {
        return Vector3(a + Vector3(_mm_set_ps1(f)) * (b - a));
    }

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
};

// 4D Vector
class Vector4
{
public:
    union
    {
        __m128 mVec;
        struct { float x, y, z, w; };
    };


    Vector4()
    {
        mVec = _mm_setzero_ps();
    }

    explicit Vector4(float inX, float inY, float inZ, float inW)
    {
        mVec = _mm_setr_ps(inX, inY, inZ, inW);
    }

    explicit Vector4(__m128 value)
    {
        mVec = value;
    }

    // Set all four components in one line
    void Set(float inX, float inY, float inZ, float inW)
    {
        mVec = _mm_setr_ps(inX, inY, inZ, inW);
    }

    // Vector addition (a + b)
    friend Vector4 operator+(const Vector4& a, const Vector4& b)
    {
        return Vector4(_mm_add_ps(a.mVec, b.mVec));
    }

    // Vector subtraction (a - b)
    friend Vector4 operator-(const Vector4& a, const Vector4& b)
    {
        return Vector4(_mm_sub_ps(a.mVec, b.mVec));
    }

    // Component-wise multiplication
    friend Vector4 operator*(const Vector4& a, const Vector4& b)
    {
        return Vector4(_mm_mul_ps(a.mVec, b.mVec));
    }

    // Scalar multiplication
    friend Vector4 operator*(const Vector4& vec, float scalar)
    {
        return Vector4(_mm_mul_ps(vec.mVec, _mm_set_ps1(scalar)));
    }

    // Scalar multiplication
    friend Vector4 operator*(float scalar, const Vector4& vec)
    {
        return Vector4(_mm_mul_ps(_mm_set_ps1(scalar), vec.mVec));
    }

    // Scalar *=
    Vector4& operator*=(float scalar)
    {
        mVec = _mm_mul_ps(mVec, _mm_set_ps1(scalar));
        return *this;
    }

    // Scalar division
    friend Vector4 operator/(const Vector4& vec, float scalar)
    {
        return Vector4(_mm_div_ps(vec.mVec, _mm_set_ps1(scalar)));
    }

    // Scalar /=
    Vector4& operator/=(float scalar)
    {
        mVec = _mm_div_ps(mVec, _mm_set_ps1(scalar));
        return *this;
    }

    // Vector +=
    Vector4& operator+=(const Vector4& right)
    {
        mVec = _mm_add_ps(mVec, right.mVec);
        return *this;
    }

    // Vector -=
    Vector4& operator-=(const Vector4& right)
    {
        mVec = _mm_sub_ps(mVec, right.mVec);
        return *this;
    }

    // Length squared of vector (as a vector)
    Vector4 LengthSq_v() const
    {
        return Vector4(_mm_mul_ps(mVec, mVec));
    }

    // Length squared of vector
    float LengthSq() const
    {
        Vector4 length = LengthSq_v();
        return length.x + length.y + length.z + length.w;
    }

    // Length of vector (as a vector)
    Vector4 Length_v() const
    {
        Vector4 length = LengthSq_v();
        return Vector4(_mm_rsqrt_ps(length.mVec));
    }

    // Length of vector
    float Length() const
    {
        Vector4 length = Length_v();
        return length.x + length.y + length.z + length.w;
    }

    // Normalize this vector
    void Normalize()
    {
        // The mask 0xF8 will dot the x, y, z, and w components,
        // and store it in result w
        __m128 temp = _mm_dp_ps(mVec, mVec, 0xF8);
        // Shuffle all values to equal w
        temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLER(3, 3, 3, 3));
        temp = _mm_rsqrt_ps(temp);
        mVec = _mm_mul_ps(mVec, temp);
    }

    // Normalize the provided vector
    friend Vector4 Normalize(const Vector4& vec)
    {
        __m128 temp = _mm_dp_ps(vec.mVec, vec.mVec, 0xF8);
        temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLER(3, 3, 3, 3));
        temp = _mm_rsqrt_ps(temp);
        _mm_shuffle_ps(temp, temp, _MM_SHUFFLER(3, 3, 3, 3));
        return Vector4(_mm_mul_ps(vec.mVec, temp));
    }

    // Dot product between two vectors (a dot b) (as a vector)
    friend Vector4 Dot_v(const Vector4& a, const Vector4& b)
    {
        return Vector4(_mm_dp_ps(a.mVec, b.mVec, 0xF8));
    }

    // Dot product between two vectors (a dot b)
    friend float Dot(const Vector4& a, const Vector4& b)
    {
        Vector4 dot = Dot_v(a, b);
        return dot.x;
    }

    // Cross product between two vectors (a cross b) (assumes w = 0)
    friend Vector4 Cross(const Vector4& a, const Vector4& b)
    {
        return Vector4();
        __m128 tempA = _mm_shuffle_ps(a.mVec, a.mVec, _MM_SHUFFLER(1, 2, 0, 0));
        __m128 tempB = _mm_shuffle_ps(b.mVec, b.mVec, _MM_SHUFFLER(2, 0, 1, 0));
        Vector4 lhs = Vector4(tempA) * Vector4(tempB);
        tempA = _mm_shuffle_ps(a.mVec, a.mVec, _MM_SHUFFLER(2, 0, 1, 0));
        tempB = _mm_shuffle_ps(b.mVec, b.mVec, _MM_SHUFFLER(1, 2, 0, 0));
        Vector4 rhs = Vector4(tempA) * Vector4(tempB);
        return lhs - rhs;
    }

    // Lerp from A to B by f
    friend Vector4 Lerp(const Vector4& a, const Vector4& b, float f)
    {
        return Vector4(a + Vector4(_mm_set_ps1(f)) * (b - a));
    }

    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;
};

// 4x4 Matrix
class Matrix4
{
public:
    union
    {
        __m128 rows[4];
        float mat[4][4];
    };

    Matrix4()
    {
        rows[0] = _mm_setzero_ps();
        rows[1] = _mm_setzero_ps();
        rows[2] = _mm_setzero_ps();
        rows[3] = _mm_setzero_ps();
    }

    explicit Matrix4(const float inMat[4][4])
    {
        memcpy(mat, inMat, 16 * sizeof(float));
    }

    explicit Matrix4(const __m128 inRows[4])
    {
        rows[0] = inRows[0];
        rows[1] = inRows[1];
        rows[2] = inRows[2];
        rows[3] = inRows[3];
    }

    // Cast to a const float pointer
    const float* GetAsFloatPtr() const
    {
        return &mat[0][0];
    }

    // Matrix multiplication (a * b)
    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        // transpose b
        __m128 bT[4];
        __m128 tmp0 = _mm_shuffle_ps(b.rows[0], b.rows[1], 0x44);
        __m128 tmp2 = _mm_shuffle_ps(b.rows[0], b.rows[1], 0xee);
        __m128 tmp1 = _mm_shuffle_ps(b.rows[2], b.rows[3], 0x44);
        __m128 tmp3 = _mm_shuffle_ps(b.rows[2], b.rows[3], 0xee);
        bT[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        bT[1] = _mm_shuffle_ps(tmp0, tmp1, 0xdd);
        bT[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        bT[3] = _mm_shuffle_ps(tmp2, tmp3, 0xdd);

        __m128 rows[4];
        for (int i = 0; i < 4; i++)
        {
            rows[i] = _mm_add_ps(
                _mm_add_ps(_mm_dp_ps(a.rows[i], bT[0], 0xF1),
                    _mm_dp_ps(a.rows[i], bT[1], 0xF2)
                ),
                _mm_add_ps(_mm_dp_ps(a.rows[i], bT[2], 0xF4),
                    _mm_dp_ps(a.rows[i], bT[3], 0xF8)
                )
            );
        }

        return Matrix4(rows);
    }

    Matrix4& operator*=(const Matrix4& right)
    {
        *this = *this * right;
        return *this;
    }

    // Invert the matrix - super slow
    void Invert();

    // Get the translation component of the matrix
    Vector3 GetTranslation() const
    {
        return Vector3(rows[3]);
    }

    // Get the X axis of the matrix (forward)
    Vector3 GetXAxis() const
    {
        return Normalize(Vector3(rows[0]));
    }

    // Get the Y axis of the matrix (left)
    Vector3 GetYAxis() const
    {
        return Normalize(Vector3(rows[1]));
    }

    // Get the Z axis of the matrix (up)
    Vector3 GetZAxis() const
    {
        return Normalize(Vector3(rows[2]));
    }

    // Extract the scale component from the matrix
    Vector3 GetScale() const
    {
        __m128 x = _mm_dp_ps(rows[0], rows[0], 0x71);
        __m128 y = _mm_dp_ps(rows[1], rows[1], 0x72);
        __m128 z = _mm_dp_ps(rows[2], rows[2], 0x74);
        return Vector3(_mm_sqrt_ps(_mm_add_ps(_mm_add_ps(x, y), z)));
    }

    // Transpose this matrix
    void Transpose()
    {
        _MM_TRANSPOSE4_PS(rows[0], rows[1], rows[2], rows[3]);
    }

    // Transpose the provided matrix
    friend Matrix4 Transpose(const Matrix4& inMat)
    {
        Matrix4 retVal = inMat;
        retVal.Transpose();
        return retVal;
    }

    // Create a scale matrix with x, y, and z scales
    static Matrix4 CreateScale(float xScale, float yScale, float zScale)
    {
        float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, zScale, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateScale(const Vector3& scaleVector)
    {
        return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
    }

    // Create a scale matrix with a uniform factor
    static Matrix4 CreateScale(float scale)
    {
        return CreateScale(scale, scale, scale);
    }

    // Rotation about x-axis
    static Matrix4 CreateRotationX(float theta)
    {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f , 0.0f },
            { 0.0f, cosf(theta), sinf(theta), 0.0f },
            { 0.0f, -sinf(theta), cosf(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about y-axis
    static Matrix4 CreateRotationY(float theta)
    {
        float temp[4][4] =
        {
            { cosf(theta), 0.0f, -sinf(theta), 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { sinf(theta), 0.0f, cosf(theta), 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    // Rotation about z-axis
    static Matrix4 CreateRotationZ(float theta)
    {
        float temp[4][4] =
        {
            { cosf(theta), sinf(theta), 0.0f, 0.0f },
            { -sinf(theta), cosf(theta), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateYawPitchRoll(float yaw, float pitch, float roll)
    {
        return Matrix4::CreateRotationZ(roll)
            * Matrix4::CreateRotationX(pitch)
            * Matrix4::CreateRotationY(yaw);
    }

    // Create a rotation matrix from a quaternion
    static Matrix4 CreateFromQuaternion(const Quaternion& q);

    static Matrix4 CreateTranslation(const Vector3& trans)
    {
        float temp[4][4] =
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, trans.z, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
    {
        Vector3 forward = Normalize(at - eye);
        Vector3 left = Normalize(Cross(up, forward));
        Vector3 newUp = Normalize(Cross(forward, left));

        float temp[4][4] =
        {
            { left.x, left.y, left.z, 0.0f },
            { newUp.x, newUp.y, newUp.z, 0.0f },
            { forward.x, forward.y, forward.z, 0.0f },
            { eye.x, eye.y, eye.z, 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreateOrtho(float width, float height, float nearZ, float farZ)
    {
        float temp[4][4] =
        {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f / (farZ - nearZ), 0.0f },
            { 0.0f, 0.0f, nearZ / (nearZ - farZ), 1.0f }
        };
        return Matrix4(temp);
    }

    static Matrix4 CreatePerspectiveFOV(float fovY, float width, float height, float nearZ, float farZ)
    {
        float yScale = 1.0f / tanf(fovY / 2.0f);
        float xScale = yScale * height / width;
        float temp[4][4] =
        {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, farZ / (farZ - nearZ), 1.0f },
            { 0.0f, 0.0f, -nearZ * farZ / (farZ - nearZ), 0.0f }
        };
        return Matrix4(temp);
    }

    static const Matrix4 Identity;
};

// 3x3 Matrix
class Matrix3
{
public:
    float mat[3][3];

    Matrix3()
    {
        *this = Matrix3::Identity;
    }

    explicit Matrix3(const float inMat[3][3])
    {
        memcpy(mat, inMat, 9 * sizeof(float));
    }

    // Cast to a const float pointer
    const float* GetAsFloatPtr() const
    {
        return reinterpret_cast<const float*>(&mat[0][0]);
    }

    // Matrix multiplication
    friend Matrix3 operator*(const Matrix3& left, const Matrix3& right)
    {
        Matrix3 retVal;
        // row 0
        retVal.mat[0][0] =
            left.mat[0][0] * right.mat[0][0] +
            left.mat[0][1] * right.mat[1][0] +
            left.mat[0][2] * right.mat[2][0];

        retVal.mat[0][1] =
            left.mat[0][0] * right.mat[0][1] +
            left.mat[0][1] * right.mat[1][1] +
            left.mat[0][2] * right.mat[2][1];

        retVal.mat[0][2] =
            left.mat[0][0] * right.mat[0][2] +
            left.mat[0][1] * right.mat[1][2] +
            left.mat[0][2] * right.mat[2][2];

        // row 1
        retVal.mat[1][0] =
            left.mat[1][0] * right.mat[0][0] +
            left.mat[1][1] * right.mat[1][0] +
            left.mat[1][2] * right.mat[2][0];

        retVal.mat[1][1] =
            left.mat[1][0] * right.mat[0][1] +
            left.mat[1][1] * right.mat[1][1] +
            left.mat[1][2] * right.mat[2][1];

        retVal.mat[1][2] =
            left.mat[1][0] * right.mat[0][2] +
            left.mat[1][1] * right.mat[1][2] +
            left.mat[1][2] * right.mat[2][2];

        // row 2
        retVal.mat[2][0] =
            left.mat[2][0] * right.mat[0][0] +
            left.mat[2][1] * right.mat[1][0] +
            left.mat[2][2] * right.mat[2][0];

        retVal.mat[2][1] =
            left.mat[2][0] * right.mat[0][1] +
            left.mat[2][1] * right.mat[1][1] +
            left.mat[2][2] * right.mat[2][1];

        retVal.mat[2][2] =
            left.mat[2][0] * right.mat[0][2] +
            left.mat[2][1] * right.mat[1][2] +
            left.mat[2][2] * right.mat[2][2];

        return retVal;
    }

    Matrix3& operator*=(const Matrix3& right)
    {
        *this = *this * right;
        return *this;
    }

    void Transpose()
    {
        float temp[3][3];

        temp[0][0] = mat[0][0];
        temp[0][1] = mat[1][0];
        temp[0][2] = mat[2][0];

        temp[1][0] = mat[0][1];
        temp[1][1] = mat[1][1];
        temp[1][2] = mat[2][1];

        temp[2][0] = mat[0][2];
        temp[2][1] = mat[1][2];
        temp[2][2] = mat[2][2];

        memcpy(mat, temp, 9 * sizeof(float));
    }

    // Transpose the provided matrix
    friend Matrix3 Transpose(const Matrix3& inMat)
    {
        Matrix3 retVal = inMat;
        retVal.Transpose();
        return retVal;
    }

    // Create a scale matrix with x and y scales
    static Matrix3 CreateScale(float xScale, float yScale)
    {
        float temp[3][3] =
        {
            { xScale, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    static Matrix3 CreateScale(const Vector2& scaleVector)
    {
        return CreateScale(scaleVector.x, scaleVector.y);
    }

    // Create a scale matrix with a uniform factor
    static Matrix3 CreateScale(float scale)
    {
        return CreateScale(scale, scale);
    }

    // Create a rotation matrix about the Z axis
    // theta is in radians
    static Matrix3 CreateRotation(float theta)
    {
        float temp[3][3] =
        {
            { cosf(theta), sinf(theta), 0.0f },
            { -sinf(theta), cosf(theta), 0.0f },
            { 0.0f, 0.0f, 1.0f },
        };
        return Matrix3(temp);
    }

    // Create a translation matrix (on the xy-plane)
    static Matrix3 CreateTranslation(const Vector2& trans)
    {
        float temp[3][3] =
        {
            { 1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { trans.x, trans.y, 1.0f },
        };
        return Matrix3(temp);
    }

    static const Matrix3 Identity;
};

// (Unit) Quaternion
class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion()
    {
        *this = Quaternion::Identity;
    }

    // This directly sets the quaternion components --
    // don't use for axis/angle
    explicit Quaternion(float inX, float inY, float inZ, float inW)
    {
        Set(inX, inY, inZ, inW);
    }

    // Construct the quaternion from an axis and angle
    // It is assumed that axis is already normalized,
    // and the angle is in radians
    explicit Quaternion(const Vector3& axis, float angle)
    {
        float scalar = sinf(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = cosf(angle / 2.0f);
    }

    // Directly set the internal components
    void Set(float inX, float inY, float inZ, float inW)
    {
        x = inX;
        y = inY;
        z = inZ;
        w = inW;
    }

    void Conjugate()
    {
        x *= -1.0f;
        y *= -1.0f;
        z *= -1.0f;
    }

    float LengthSq() const
    {
        return (x * x + y * y + z * z + w * w);
    }

    float Length() const
    {
        return sqrtf(LengthSq());
    }

    void Normalize()
    {
        float length = Length();
        x /= length;
        y /= length;
        z /= length;
        w /= length;
    }

    // Normalize the provided quaternion
    friend Quaternion Normalize(const Quaternion& q)
    {
        Quaternion retVal = q;
        retVal.Normalize();
        return retVal;
    }

    // Linear interpolation
    friend Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f)
    {
        const float dotResult = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        float bias = -1.0f;
        if (dotResult >= 0.0f)
        {
            bias = 1.0f;
        }

        Quaternion retVal;
        retVal.x = b.x * f + a.x * bias * (1.0f - f);
        retVal.y = b.y * f + a.y * bias * (1.0f - f);
        retVal.z = b.z * f + a.z * bias * (1.0f - f);
        retVal.w = b.w * f + a.w * bias * (1.0f - f);
        retVal.Normalize();
        return retVal;
    }

    // Spherical Linear Interpolation
    friend Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f);

    // Concatenate
    // Rotate by q FOLLOWED BY p
    friend Quaternion Concatenate(const Quaternion& q, const Quaternion& p)
    {
        Quaternion retVal;

        // Vector component is:
        // ps * qv + qs * pv + pv x qv
        Vector3 qv(q.x, q.y, q.z);
        Vector3 pv(p.x, p.y, p.z);
        Vector3 newVec = p.w * qv + q.w * pv + Cross(pv, qv);
        retVal.x = newVec.x;
        retVal.y = newVec.y;
        retVal.z = newVec.z;

        // Scalar component is:
        // ps * qs - pv . qv
        retVal.w = p.w * q.w - Dot(pv, qv);

        return retVal;
    }

    // Dot product between two quaternions
    friend float Dot(const Quaternion& a, const Quaternion& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    static const Quaternion Identity;
};

// Inline implementations of transform functions that rely on matrices
inline Vector2 Transform(const Vector2& vec, const Matrix3& mat, float z = 0.0f)
{
    Vector2 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] + z * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] + z * mat.mat[2][1];
    //ignore z since we aren't returning a new value for it...
    return retVal;
}

inline Vector3 Transform(const Vector3& vec, const Matrix3& mat)
{
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
        vec.z * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
        vec.z * mat.mat[2][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
        vec.z * mat.mat[2][2];
    return retVal;
}

inline Vector3 Transform(const Vector3& vec, const Matrix4& mat, float w = 1.0f)
{
    Vector3 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
        vec.z * mat.mat[2][0] + w * mat.mat[3][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
        vec.z * mat.mat[2][1] + w * mat.mat[3][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
        vec.z * mat.mat[2][2] + w * mat.mat[3][2];
    return retVal;
}

inline Vector4 Transform(const Vector4& vec, const Matrix4& mat)
{
    Vector4 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] +
        vec.z * mat.mat[2][0] + vec.w * mat.mat[3][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] +
        vec.z * mat.mat[2][1] + vec.w * mat.mat[3][1];
    retVal.z = vec.x * mat.mat[0][2] + vec.y * mat.mat[1][2] +
        vec.z * mat.mat[2][2] + vec.w * mat.mat[3][2];
    retVal.w = vec.x * mat.mat[0][3] + vec.y * mat.mat[1][3] +
        vec.z * mat.mat[2][3] + vec.w * mat.mat[3][3];
    return retVal;
}

// Transform a Vector3 by a quaternion
inline Vector3 Transform(const Vector3& v, const Quaternion& q)
{
    // v + 2.0*cross(q.xyz, cross(q.xyz,v) + q.w*v);
    Vector3 qv(q.x, q.y, q.z);
    Vector3 retVal = v;
    retVal += 2.0f * Cross(qv, Cross(qv, v) + q.w * v);
    return retVal;
}