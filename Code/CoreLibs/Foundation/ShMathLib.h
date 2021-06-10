#pragma once


SH_NAMESPACE_BEGIN


#define INT8_SIGN_BIT               7
#define INT16_SIGN_BIT              15
#define INT32_SIGN_BIT              31
#define INT64_SIGN_BIT              63

#define INT8_SIGN_MASK              (1 << INT8_SIGN_BIT)
#define INT16_SIGN_MASK             (1 << INT16_SIGN_BIT)
#define INT32_SIGN_MASK             (1UL << INT32_SIGN_BIT)
#define INT64_SIGN_MASK             (1ULL << INT64_SIGN_BIT)

// integer bit detection
#define INT64_SIGNBITSET(i)         (((const uint64_t)(i)) >> INT64_SIGN_BIT)
#define INT64_SIGNBITNOTSET(i)      ((~((const uint64_t)(i))) >> INT64_SIGN_BIT)
#define INT32_SIGNBITSET(i)         (((const uint32_t)(i)) >> INT32_SIGN_BIT)
#define INT32_SIGNBITNOTSET(i)      ((~((const uint32_t)(i))) >> INT32_SIGN_BIT)
#define INT16_SIGNBITSET(i)         (((const uint16_t)(i)) >> INT16_SIGN_BIT)
#define INT16_SIGNBITNOTSET(i)      ((~((const uint16_t)(i))) >> INT16_SIGN_BIT)

template <typename T>
__forceinline void Clamp(T& v, const T& min, const T& max) { v = (v > max) ? max : (v < min ? min : v); }
/// Clamps a number to the range [0, 1].
template <typename T>
__forceinline void Clamp01(T& v) { Clamp(v, T(0), T(1)); }

/// Returns the clamped number to a range.
template <typename T>
__forceinline constexpr T Clamp(const T& v, const T& min, const T& max) { return (v > max) ? max : (v < min ? min : v); }
/// Returns the clamped number to the range [0, 1].
template <typename T>
__forceinline constexpr T Clamp01(const T& v) { return Clamp(v, T(0), T(1)); }

/*
-------------------------------------------------------------------------------

floating point bit layouts according to the IEEE 754-1985 and 754-2008 standard

-------------------------------------------------------------------------------
*/

// IEEE half float - 16 bit
#define IEEE_FLT16_MANTISSA_BITS    10
#define IEEE_FLT16_EXPONENT_BITS    5
#define IEEE_FLT16_EXPONENT_BIAS    15
#define IEEE_FLT16_SIGN_BIT         15

// IEEE single float - 32 bit
#define IEEE_FLT_MANTISSA_BITS      23
#define IEEE_FLT_EXPONENT_BITS      8
#define IEEE_FLT_EXPONENT_BIAS      127
#define IEEE_FLT_SIGN_BIT           31

// IEEE double - 64 bit
#define IEEE_DBL_MANTISSA_BITS      52
#define IEEE_DBL_EXPONENT_BITS      11
#define IEEE_DBL_EXPONENT_BIAS      1023
#define IEEE_DBL_SIGN_BIT           63

// IEEE double extended - 80 bit
#define IEEE_DBLE_MANTISSA_BITS     64
#define IEEE_DBLE_EXPONENT_BITS     15
#define IEEE_DBLE_EXPONENT_BIAS     16383
#define IEEE_DBLE_SIGN_BIT          79

// IEEE quad double - 128 bit
#define IEEE_QDBL_MANTISSA_BITS     112
#define IEEE_QDBL_EXPONENT_BITS     15
#define IEEE_QDBL_EXPONENT_BIAS     16383
#define IEEE_QDBL_SIGN_BIT          127

// float bit detection
#define IEEE_FLT_SIGNBITSET(f)      ((*(const uint32_t *)&(f)) >> 31)
#define IEEE_FLT_SIGNBITNOTSET(f)   ((~(*(const uint32_t *)&(f))) >> 31)
#define IEEE_FLT_NOTZERO(f)         ((*(const uint32_t *)&(f)) & ~(1 << 31))

// float NaN, INF, IND, DENORMAL detection (INF, IND is included in NaN)
#define IEEE_FLT_IS_NAN(x)          (((*(const uint32_t *)&x) & 0x7f800000) == 0x7f800000)
#define IEEE_FLT_IS_INF(x)          (((*(const uint32_t *)&x) & 0x7fffffff) == 0x7f800000)
#define IEEE_FLT_IS_IND(x)          ((*(const uint32_t *)&x) == 0xffc00000)
#define IEEE_FLT_IS_DENORMAL(x)     (((*(const uint32_t *)&x) & 0x7f800000) == 0x00000000 && \
                                    ((*(const uint32_t *)&x) & 0x007fffff) != 0x00000000)
// degree <-> radian
#define DEG2RAD(a)                  ((a) * BE1::Math::MulDegreeToRadian)
#define RAD2DEG(a)                  ((a) * BE1::Math::MulRadianToDegree)

// seconds <-> miliseconds
#define SEC2MILLI(t)                ((t) * BE1::Math::MulSecondToMilli)
#define MILLI2SEC(t)                ((t) * BE1::Math::MulMilliToSecond)

// seconds <-> microseconds
#define SEC2MICRO(t)                ((t) * BE1::Math::MulSecondToMicro)
#define MICRO2SEC(t)                ((t) * BE1::Math::MulMicroToSecond)

// seconds <-> nanoseconds
#define SEC2NANO(t)                 ((t) * BE1::Math::MulSecondToNano)
#define NANO2SEC(t)                 ((t) * BE1::Math::MulNanoToSecond)

// degree <-> short value
#define ANGLE2SHORT(x)              (BE1::Math::FtoiFast((x) * 65536.0f / 360.0f) & 65535)
#define SHORT2ANGLE(x)              ((x) * (360.0f / 65536.0f))

// degree <-> byte value            
#define ANGLE2BYTE(x)               (BE1::Math::FtoiFast((x) * 256.0f / 360.0f) & 255)
#define BYTE2ANGLE(x)               ((x) * (360.0f / 256.0f))

#define C_FLOAT_TO_INT(x)           (int)(x)


template <typename T>
__forceinline T Sign(const T x) { return (x > 0) ? 1 : ((x < 0) ? -1 : 0); }
template <typename T>
__forceinline T Square(const T& x) { return x * x; }
template <typename T>
__forceinline T Cube(const T& x) { return x * x * x; }

template <typename T>
__forceinline T InchesToMetres(const T x) { return static_cast<T>(x * 0.0254f); }
template <typename T>
__forceinline T MetresToInches(const T x) { return static_cast<T>(x * 39.37f); }
template <typename T>
__forceinline T InchesToFeet(const T x) { return static_cast<T>(x / 12.f); }
template <typename T>
__forceinline T FeetToMiles(const T x) { return static_cast<T>(x / 5280.f); }
template <typename T>
__forceinline T FeetToInches(const T x) { return static_cast<T>(x * 12.f); }
template <typename T>
__forceinline T MetresToFeet(const T x) { return InchesToFeet(MetresToInches(x)); }
template <typename T>
__forceinline T FeetToMetres(const T x) { return FeetToInches(InchesToMetres(x)); }

template <unsigned int Value>
struct Factorial {
    enum { Answer = Value * Factorial<Value - 1>::Answer };
};

template<>
struct Factorial<0> {
    enum { Answer = 1 };
};


class Math
{
public:
	static const float Pi;
    static const float TwoPi;
    static const float HalfPi;
    static const float OneFourthPi;
	static const float InvPi;
    static const float InvTwoPi;
	static const float E;
	static const float SqrtTwo;
	static const float Infinity;

	static float Sqrt(float x);
	static float Sqrt16(float x);
	static double Sqrt64(float x);

	static float Sin(float a);
	static float Sin16(float a);
	static double Sin64(float a);

	static float Cos(float a);
	static float Cos16(float a);
	static double Cos64(float a);

    static void SinCos(float a, float& s, float& c);
    static void SinCos16(float a, float& s, float& c);
    static void SinCos64(float a, double& s, double& c);

	static float Tan(float a);
	static float Tan16(float a);
	static double Tan64(float a);

    static float ASin(float a);
    static float ASin16(float a);
    static double ASin64(float a);

    static float ACos(float a);
    static float ACos16(float a);
    static double ACos64(float a);

    static float ATan(float a);
    static float ATan16(float a);
    static double ATan64(float a);

    static float ATan(float y, float x);
    static float ATan16(float y, float x);
    static double ATan64(float y, float x);

    static float Pow(float x, float y);
    static float Pow16(float x, float y);
    static double Pow64(float x, float y);

    static float Exp(float f);
    static float Exp16(float f);
    static double Exp64(float f);

    static float Ln(float f);
    static float Ln16(float f);
    static double Ln64(float f);

    static float Log(float b, float f);
    static float Log16(float b, float f);
    static double Log64(float b, float f);

    static int IPow(int x, int y);
    static int ILog2(float f);
    static int ILog2(int i);

    static int BitsForFloat(float f);
    static int BitsForInteger(int i);

    static bool IsPowerOfTwo(int x);
    static int RoundDownPowerOfTwo(int x);
    static int RoundUpPowerOfTwo(int x);

    static int Abs(int x);
    static float Fabs(float f);

    static float Floor(float f);
    static float Ceil(float f);
    static float Round(float f);
    static float Fract(float f);

    static int Ftoi(float f);
    static int FtoiFast(float f);
    static int8_t Ftoi8(float f);
    static int16_t Ftoi16(float f);
    static uint16_t Ftoui16(float f);

    static double Factorial(unsigned int n);
    static float Snap(float value, float snapSize);

    static float AngleNormalize360(float angle);
    static float AngleNormalize180(float angle);
    static float AngleDelta(float angle1, float angle2);

    static float FlipAngleX(float angle);
    static float FlipAngleY(float angle);

    static int FloatHash(const float* arr, const int numFloats);

    static float Random(float minimum, float maximum);

    static float SmoothStep(float edge0, float edge1, float x);

    template <typename T>
    static T Lerp(const T p0, const T p1, float t);
    static int FixedLerp(int p0, int p1, int t);

    template <typename T>
    static T Cerp(const T p0, const T p1, const T p2, const T p3, float t);
    static int FixedCerp(int p0, int p1, int p2, int p3, int t);

};


inline float Math::Sqrt(float x) {
	return (x >= 0.0f) ? sqrtf(x) : 0.0f;
}

inline float Math::Sqrt16(float x) {
	return (x >= 0.0f) ? sqrtf(x) : 0.0f;
}

inline double Math::Sqrt64(float x) {
	return (x >= 0.0f) ? sqrtf(x) : 0.0f;
}


inline float Math::Sin(float a) {
    return sinf(a);
}

inline float Math::Sin16(float a) {
    float s;

    if ((a < 0.0f) || (a >= TwoPi)) {
        a -= floorf(a * InvTwoPi) * TwoPi;
    }
#if 1
    if (a < Pi) {
        if (a > HalfPi) {
            a = Pi - a;
        }
    }
    else {
        if (a > Pi + HalfPi) {
            a = a - TwoPi;
        }
        else {
            a = Pi - a;
        }
    }
#else
    a = Pi - a;
    if (fabs(a) >= HalfPi) {
        a = ((a < 0.0f) ? -Pi : Pi) - a;
    }
#endif
    s = a * a;
    return a * (((((-2.39e-08f * s + 2.7526e-06f) * s - 1.98409e-04f) * s + 8.3333315e-03f) * s - 1.666666664e-01f) * s + 1.0f);
}

inline double Math::Sin64(float a) {
    return sin(a);
}

inline float Math::Cos(float a) {
    return cosf(a);
}

inline float Math::Cos16(float a) {
    float s, d;

    if ((a < 0.0f) || (a >= TwoPi)) {
        a -= floorf(a * InvTwoPi) * TwoPi;
    }
#if 1
    if (a < Pi) {
        if (a > HalfPi) {
            a = Pi - a;
            d = -1.0f;
        }
        else {
            d = 1.0f;
        }
    }
    else {
        if (a > Pi + HalfPi) {
            a = a - TwoPi;
            d = 1.0f;
        }
        else {
            a = Pi - a;
            d = -1.0f;
        }
    }
#else
    a = Pi - a;
    if (fabs(a) >= HalfPi) {
        a = ((a < 0.0f) ? -Pi : Pi) - a;
        d = 1.0f;
    }
    else {
        d = -1.0f;
    }
#endif
    s = a * a;
    return d * (((((-2.605e-07f * s + 2.47609e-05f) * s - 1.3888397e-03f) * s + 4.16666418e-02f) * s - 4.999999963e-01f) * s + 1.0f);
}

inline double Math::Cos64(float a) {
    return cos(a);
}


inline void Math::SinCos(float a, float& s, float& c) {
#if defined(__WIN32__) && !defined(__X86_64__)
    _asm {
        fld     a
        fsincos
        mov     ecx, c
        mov     edx, s
        fstp    dword ptr[ecx]
        fstp    dword ptr[edx]
    }
#else
    s = sinf(a);
    c = cosf(a);
#endif
}

inline void Math::SinCos16(float a, float& s, float& c) {
    float t, d;

    if ((a < 0.0f) || (a >= Math::TwoPi)) {
        a -= floorf(a / Math::TwoPi) * Math::TwoPi;
    }
#if 1
    if (a < Pi) {
        if (a > HalfPi) {
            a = Pi - a;
            d = -1.0f;
        }
        else {
            d = 1.0f;
        }
    }
    else {
        if (a > Pi + HalfPi) {
            a = a - TwoPi;
            d = 1.0f;
        }
        else {
            a = Pi - a;
            d = -1.0f;
        }
    }
#else
    a = Pi - a;
    if (fabs(a) >= HalfPi) {
        a = ((a < 0.0f) ? -Pi : Pi) - a;
        d = 1.0f;
    }
    else {
        d = -1.0f;
    }
#endif
    t = a * a;
    s = a * (((((-2.39e-08f * t + 2.7526e-06f) * t - 1.98409e-04f) * t + 8.3333315e-03f) * t - 1.666666664e-01f) * t + 1.0f);
    c = d * (((((-2.605e-07f * t + 2.47609e-05f) * t - 1.3888397e-03f) * t + 4.16666418e-02f) * t - 4.999999963e-01f) * t + 1.0f);
}

inline void Math::SinCos64(float a, double& s, double& c) {
#if defined(__WIN32__) && !defined(__X86_64__)
    _asm {
        fld		a
        fsincos
        mov		ecx, c
        mov		edx, s
        fstp	qword ptr[ecx]
        fstp	qword ptr[edx]
    }
#else
    s = sin(a);
    c = cos(a);
#endif
}


inline float Math::Tan(float a) {
    return tanf(a);
}

inline float Math::Tan16(float a) {
    float s;
    bool reciprocal;

    if ((a < 0.0f) || (a >= Pi)) {
        a -= floorf(a / Pi) * Pi;
    }
#if 1
    if (a < HalfPi) {
        if (a > OneFourthPi) {
            a = HalfPi - a;
            reciprocal = true;
        }
        else {
            reciprocal = false;
        }
    }
    else {
        if (a > HalfPi + OneFourthPi) {
            a = a - Pi;
            reciprocal = false;
        }
        else {
            a = HalfPi - a;
            reciprocal = true;
        }
    }
#else
    a = HalfPi - a;
    if (fabs(a) >= OneFourthPi) {
        a = ((a < 0.0f) ? -HalfPi : HalfPi) - a;
        reciprocal = false;
    }
    else {
        reciprocal = true;
    }
#endif
    s = a * a;
    s = a * ((((((9.5168091e-03f * s + 2.900525e-03f) * s + 2.45650893e-02f) * s + 5.33740603e-02f) * s + 1.333923995e-01f) * s + 3.333314036e-01f) * s + 1.0f);
    if (reciprocal) {
        return 1.0f / s;
    }
    else {
        return s;
    }
}

inline double Math::Tan64(float a) {
    return tan(a);
}


inline float Math::ASin(float a) {
    if (a <= -1.0f) {
        return -HalfPi;
    }
    if (a >= 1.0f) {
        return HalfPi;
    }
    return asinf(a);
}

inline float Math::ASin16(float a) {
    if (IEEE_FLT_SIGNBITSET(a)) {
        if (a <= -1.0f) {
            return -HalfPi;
        }
        a = fabs(a);
        return (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrt(1.0f - a) - HalfPi;
    }
    else {
        if (a >= 1.0f) {
            return HalfPi;
        }
        return HalfPi - (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrt(1.0f - a);
    }
}

inline double Math::ASin64(float a) {
    if (a <= -1.0f) {
        return -HalfPi;
    }
    if (a >= 1.0f) {
        return HalfPi;
    }
    return asin(a);
}

inline float Math::ACos(float a) {
    if (a <= -1.0f) {
        return Pi;
    }
    if (a >= 1.0f) {
        return 0.0f;
    }
    return acosf(a);
}

inline float Math::ACos16(float a) {
    if (IEEE_FLT_SIGNBITSET(a)) {
        if (a <= -1.0f) {
            return Pi;
        }
        a = fabs(a);
        return Pi - (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrt(1.0f - a);
    }
    else {
        if (a >= 1.0f) {
            return 0.0f;
        }
        return (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrt(1.0f - a);
    }
}

inline double Math::ACos64(float a) {
    if (a <= -1.0f) {
        return Pi;
    }
    if (a >= 1.0f) {
        return 0.0f;
    }
    return acos(a);
}

inline float Math::ATan(float a) {
    return atanf(a);
}

inline float Math::ATan16(float a) {
    float s;

    if (fabs(a) > 1.0f) {
        a = 1.0f / a;
        s = a * a;
        s = -(((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
        if (IEEE_FLT_SIGNBITSET(a)) {
            return s - HalfPi;
        }
        else {
            return s + HalfPi;
        }
    }
    else {
        s = a * a;
        return (((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
    }
}

inline double Math::ATan64(float a) {
    return atan(a);
}

inline float Math::ATan(float y, float x) {
    return atan2f(y, x);
}

inline float Math::ATan16(float y, float x) {
    float a, s;

    if (fabs(y) > fabs(x)) {
        a = x / y;
        s = a * a;
        s = -(((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
        if (IEEE_FLT_SIGNBITSET(a)) {
            return s - HalfPi;
        }
        else {
            return s + HalfPi;
        }
    }
    else {
        a = y / x;
        s = a * a;
        return (((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
    }
}

inline double Math::ATan64(float y, float x) {
    return atan2(y, x);
}

inline float Math::Pow(float x, float y) {
    return powf(x, y);
}

inline float Math::Pow16(float x, float y) {
    return Exp16(y * Ln16(x));
}

inline double Math::Pow64(float x, float y) {
    return pow(x, y);
}

inline float Math::Exp(float f) {
    return expf(f);
}

inline float Math::Exp16(float f) {
    float x = f * 1.44269504088896340f; // multiply with ( 1 / log( 2 ) )
#if 1
    int i = *reinterpret_cast<int*>(&x);
    int s = (i >> IEEE_FLT_SIGN_BIT);
    int e = ((i >> IEEE_FLT_MANTISSA_BITS) & ((1 << IEEE_FLT_EXPONENT_BITS) - 1)) - IEEE_FLT_EXPONENT_BIAS;
    int m = (i & ((1 << IEEE_FLT_MANTISSA_BITS) - 1)) | (1 << IEEE_FLT_MANTISSA_BITS);
    i = ((m >> (IEEE_FLT_MANTISSA_BITS - e)) & ~(e >> 31)) ^ s;
#else
    i = (int)x;
    if (x < 0.0f)
        i--;
#endif
    int exponent = (i + IEEE_FLT_EXPONENT_BIAS) << IEEE_FLT_MANTISSA_BITS;
    float y = *reinterpret_cast<float*>(&exponent);
    x -= (float)i;
    if (x >= 0.5f) {
        x -= 0.5f;
        y *= 1.4142135623730950488f;    // multiply with sqrt( 2 )
    }
    float x2 = x * x;
    float p = x * (7.2152891511493f + x2 * 0.0576900723731f);
    float q = 20.8189237930062f + x2;
    x = y * (q + p) / (q - p);
    return x;
}

inline double Math::Exp64(float f) {
    return exp(f);
}

inline float Math::Ln(float f) {
    return logf(f);
}

inline float Math::Ln16(float f) {
    int i = *reinterpret_cast<int*>(&f);
    int exponent = ((i >> IEEE_FLT_MANTISSA_BITS) & ((1 << IEEE_FLT_EXPONENT_BITS) - 1)) - IEEE_FLT_EXPONENT_BIAS;
    i -= (exponent + 1) << IEEE_FLT_MANTISSA_BITS; // get value in the range [.5, 1>
    float y = *reinterpret_cast<float*>(&i);
    y *= 1.4142135623730950488f;                    // multiply with sqrt( 2 )
    y = (y - 1.0f) / (y + 1.0f);
    float y2 = y * y;
    y = y * (2.000000000046727f + y2 * (0.666666635059382f + y2 * (0.4000059794795f + y2 * (0.28525381498f + y2 * 0.2376245609f))));
    y += 0.693147180559945f * ((float)exponent + 0.5f);
    return y;
}

inline double Math::Ln64(float f) {
    return log(f);
}

inline float Math::Log(float b, float f) {
    return Ln(f) / Ln(b);
}

inline float Math::Log16(float b, float f) {
    return Ln16(f) / Ln16(b);
}

inline double Math::Log64(float b, float f) {
    return Ln64(f) / Ln64(b);
}

inline int Math::IPow(int x, int y) {
    int r;
    for (r = x; y > 1; y--) {
        r *= x;
    }
    return r;
}

inline int Math::ILog2(float f) {
    return (((*reinterpret_cast<int*>(&f)) >> IEEE_FLT_MANTISSA_BITS) & ((1 << IEEE_FLT_EXPONENT_BITS) - 1)) - IEEE_FLT_EXPONENT_BIAS;
}

inline int Math::ILog2(int i) {
    return ILog2((float)i);
}

inline int Math::BitsForFloat(float f) {
    return ILog2(f) + 1;
}

inline int Math::BitsForInteger(int i) {
    return ILog2((float)i) + 1;
}

inline bool Math::IsPowerOfTwo(int x) {
    return (x & (x - 1)) == 0 && x > 0;
}

inline int Math::RoundDownPowerOfTwo(int x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x >>= 1;
    x++;
    return x;
}

inline int Math::RoundUpPowerOfTwo(int x) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

inline int Math::Abs(int x) {
    int y = x >> 31;
    return ((x ^ y) - y);
}

inline float Math::Fabs(float f) {
#if 0
    if (f >= 0) {
        return f;
    }
    return -f;
#else
    int tmp = *reinterpret_cast<int*>(&f);
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float*>(&tmp);
#endif
}

inline float Math::Floor(float f) {
    return floorf(f);
}

inline float Math::Ceil(float f) {
    return ceilf(f);
}

inline float Math::Fract(float f) {
    return f - floorf(f);
}

inline float Math::Round(float f) {
    return floorf(f + 0.5f);
}

inline int Math::Ftoi(float f) {
    return (int)f;
}

inline int Math::FtoiFast(float f) {
#ifdef HAVE_X86_SSE_INTRIN
    // If a converted result is larger than the maximum signed doubleword integer,
    // the floating-point invalid exception is raised, and if this exception is masked,
    // the indefinite integer value (80000000H) is returned.
    __m128 x = _mm_load_ss(&f);
    return _mm_cvttss_si32(x);
#elif 0 // round chop (C/C++ standard)
    int i = *reinterpret_cast<int*>(&f);
    int s = i >> IEEE_FLT_SIGN_BIT;
    int e = ((i >> IEEE_FLT_MANTISSA_BITS) & ((1 << IEEE_FLT_EXPONENT_BITS) - 1)) - IEEE_FLT_EXPONENT_BIAS;
    int m = (i & ((1 << IEEE_FLT_MANTISSA_BITS) - 1)) | (1 << IEEE_FLT_MANTISSA_BITS);
    int shift = e - IEEE_FLT_MANTISSA_BITS;
    return ((((m >> -shift) | (m << shift)) & ~(e >> 31)) ^ s) - s;
#else
    // If a converted result is larger than the maximum signed doubleword integer the result is undefined.
    return C_FLOAT_TO_INT(f);
#endif
}

inline int8_t Math::Ftoi8(float f) {
#ifdef HAVE_X86_SSE_INTRIN
    __m128 x = _mm_load_ss(&f);
    x = _mm_max_ss(x, SIMD_4::F4_min_char);
    x = _mm_min_ss(x, SIMD_4::F4_max_char);
    return static_cast<int8_t>(_mm_cvttss_si32(x));
#else
    // The converted result is clamped to the range [-128, 127].
    int i = C_FLOAT_TO_INT(f);
    if (i < -128) {
        return -128;
    }
    else if (i > 127) {
        return 127;
    }
    return static_cast<int8_t>(i);
#endif
}

inline int16_t Math::Ftoi16(float f) {
#ifdef HAVE_X86_SSE_INTRIN
    __m128 x = _mm_load_ss(&f);
    x = _mm_max_ss(x, SIMD_4::F4_min_short);
    x = _mm_min_ss(x, SIMD_4::F4_max_short);
    return static_cast<int16_t>(_mm_cvttss_si32(x));
#else
    // The converted result is clamped to the range [-32768, 32767].
    int i = C_FLOAT_TO_INT(f);
    if (i < -32768) {
        return -32768;
    }
    else if (i > 32767) {
        return 32767;
    }
    return static_cast<int16_t>(i);
#endif
}

inline uint16_t Math::Ftoui16(float f) {
    // The converted result is clamped to the range [0, 65535].
    int i = C_FLOAT_TO_INT(f);
    if (i < 0) {
        return 0;
    }
    else if (i > 65535) {
        return 65535;
    }
    return static_cast<uint16_t>(i);
}


inline double Math::Factorial(unsigned int n) {
    const double table[16] = { 1., 1., 2., 6., 24., 120., 720., 5040., 40320., 362880., 3628800., 39916800., 479001600., 6227020800., 87178291200., 1307674368000. };
    double result = table[(n > 15) ? 15 : n];

    while (n >= 16) {
        result = result * double(n);
        n--;
    }

    return result;
}

inline float Math::Snap(float value, float snapSize) {
    return Math::Round(value / snapSize) * snapSize;
}

inline float Math::AngleNormalize360(float angle) {
    if ((angle >= 360.0f) || (angle < 0.0f)) {
        angle -= floor(angle / 360.0f) * 360.0f;
    }
    return angle;
}

inline float Math::AngleNormalize180(float angle) {
    angle = AngleNormalize360(angle);
    if (angle > 180.0f) {
        angle -= 360.0f;
    }

    return angle;
}

inline float Math::AngleDelta(float angle1, float angle2) {
    return AngleNormalize180(angle1 - angle2);
}

inline float Math::FlipAngleX(float angle) {
    float s, c;
    SinCos(angle, s, c);
    return ATan(s, -c);
}

inline float Math::FlipAngleY(float angle) {
    float s, c;
    SinCos(angle, s, c);
    return ATan(-s, c);
}

inline int Math::FloatHash(const float* arr, const int numFloats) {
    int i, hash = 0;
    const int* ptr;

    ptr = reinterpret_cast<const int*>(arr);
    for (i = 0; i < numFloats; i++) {
        hash ^= ptr[i];
    }

    return hash;
}

inline float Math::Random(float minimum, float maximum) {
    return minimum + (maximum - minimum) * ((float)::rand() / (float)RAND_MAX);
}

inline float Math::SmoothStep(float edge0, float edge1, float x) {
    float t = Clamp01((x - edge0) / (edge1 - edge0));
    return t * t * (3.0 - 2.0 * t);
}

template <typename T>
inline T Math::Lerp(const T p0, const T p1, float t) {
    return p0 + ((p1 - p0) * t);
}

// Fixed point linear interpolation
// f(x) = ax + b
// p0: f(0)
// p1: f(1)
// t: interpolater in range [0, 255]
inline int Math::FixedLerp(int p0, int p1, int t) {
    // p0 : f(0) = b
    // p1 : f(1) = a + b
    //
    // a = p1 - p0
    // b = p0
    return ((p0 << 8) + (p1 - p0) * t) >> 8;
}

template <typename T>
inline T Math::Cerp(const T p0, const T p1, const T p2, const T p3, float t) {
    T a = p3 - 3 * p2 + 3 * p1 - p0;
    T b = p2 - 2 * p1 + p0 - a; // simplified version of (-p3 + 4 * p2 - 5 * p1 + 2 * p0)
    T c = p2 - p0;
    T d = 2 * p1;
    return (t * (t * (t * a + b) + c) + d) * 0.5f;
}

// Fixed point cubic interpolation
// f(x) = ax^3 + bx^2 + cx + d
// p0: f(-1)
// p1: f(0)
// p2: f(1)
// p3: f(2)
// t: interpolater in range [0, 127]
inline int Math::FixedCerp(int p0, int p1, int p2, int p3, int t) {
#if 0
    // Hermite cubic spline with 4 points
    // f'(x) = 3ax^2 + 2bx + c
    //
    // p1 : f(0) = d
    // p2 : f(1) = a + b + c + d 
    // p2 - p0 : f'(0) = c
    // p3 - p1 : f'(1) = 3a + 2b + c
    //
    // |  0  0  0  1 | | a |   | p1      |
    // |  1  1  1  1 | | b | = | p2      |
    // |  0  0  1  0 | | c |   | p2 - p0 |
    // |  3  2  1  0 | | d |   | p3 - p1 |
    //
    // |  2 -2  1  1 | | p1      |   | a | 
    // | -3  3 -2 -1 | | p2      | = | b |
    // |  0  0  1  0 | | p2 - p0 |   | c |
    // |  1  0  0  0 | | p3 - p1 |   | d |
    //
    // a = 2*p1 - 2*p2 + (p2 - p0) + (p3 - p1) = p3 - p2 + p1 - p0
    // b = -3*p1 + 3*p2 - 2*(p2 - p0) - (p3 - p1) = -p3 + p2 - 2*p1 + 2*p0
    // c = p2 - p0
    // d = p1
    int p01 = p0 - p1;
    int a = (p3 - p2) - p01;
    int b = p01 - a;
    int c = p2 - p0;
    int d = p1;
    return (t * (t * (t * a + (b << 7)) + (c << 14)) + (d << 21)) >> 21;
#else
    // Catmull-Rom cubic spline with 4 points
    // f'(x) = 3ax^2 + 2bx + c
    //
    // p1 : f(0) = d
    // p2 : f(1) = a + b + c + d
    // (p2 - p0)/2 : f'(0) = c
    // (p3 - p1)/2 : f'(1) = 3a + 2b + c
    //
    // |  0  0  0  1 | | a |   | p1      |
    // |  1  1  1  1 | | b | = | p2      |
    // |  0  0  2  0 | | c |   | p2 - p0 |
    // |  6  4  2  0 | | d |   | p3 - p1 |
    //
    //    |  4 -4  1  1 | | p1      |   | a | 
    // 1/2| -6  6 -2 -1 | | p2      | = | b |
    //    |  0  0  1  0 | | p2 - p0 |   | c |
    //    |  2  0  0  0 | | p3 - p1 |   | d |
    //
    // a = 4*p1 - 4*p2 + (p2 - p0) + (p3 - p1) = (p3 - 3*p2 + 3*p1 - p0) / 2
    // b = -6*p1 + 6*p2 - 2*(p2 - p0) - (p3 - p1) = (-p3 + 4*p2 - 5p1 + 2*p0) / 2
    // c = (p2 - p0) / 2
    // d = p1
    int a = p3 - 3 * p2 + 3 * p1 - p0;
    int b = p2 - 2 * p1 + p0 - a;
    int c = p2 - p0;
    int d = 2 * p1;
    return (t * (t * (t * a + (b << 7)) + (c << 14)) + (d << 21)) >> 22;
#endif
}


SH_NAMESPACE_END