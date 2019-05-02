/* 
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2014 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

// VCL configuration
#include <vcl/config/global.h>
#include <vcl/config/eigen.h>

// C++ Standard Library
#include <array>
#include <initializer_list>

// VCL
#include <vcl/core/contract.h>
#include <vcl/core/preprocessor.h>
#include <vcl/math/math.h>

namespace Vcl
{
	//! \addtogroup group_vcl_core_simd
	template<typename Scalar, int Width>
	class VectorScalar
	{
	};
}

#if defined(VCL_VECTORIZE_SSE) || defined(VCL_VECTORIZE_AVX)
#	include <vcl/core/simd/bool4_sse.h>
#	include <vcl/core/simd/float4_sse.h>
#	include <vcl/core/simd/int4_sse.h>
#endif

#if defined VCL_VECTORIZE_AVX
#	include <vcl/core/simd/bool8_avx.h>
#	include <vcl/core/simd/bool16_avx.h>
#	include <vcl/core/simd/float8_avx.h>
#	include <vcl/core/simd/float16_avx.h>
#	include <vcl/core/simd/int8_avx.h>
#	include <vcl/core/simd/int16_avx.h>
#elif defined VCL_VECTORIZE_SSE
#	include <vcl/core/simd/bool8_sse.h>
#	include <vcl/core/simd/bool16_sse.h>
#	include <vcl/core/simd/float8_sse.h>
#	include <vcl/core/simd/float16_sse.h>
#	include <vcl/core/simd/int8_sse.h>
#	include <vcl/core/simd/int16_sse.h>
#elif defined VCL_VECTORIZE_NEON
#	include <vcl/core/simd/bool4_neon.h>
#	include <vcl/core/simd/bool8_neon.h>
#	include <vcl/core/simd/bool16_neon.h>
#	include <vcl/core/simd/float4_neon.h>
#	include <vcl/core/simd/float8_neon.h>
#	include <vcl/core/simd/float16_neon.h>
#	include <vcl/core/simd/int4_neon.h>
#	include <vcl/core/simd/int8_neon.h>
#	include <vcl/core/simd/int16_neon.h>
#else
#	include <vcl/core/simd/bool4_ref.h>
#	include <vcl/core/simd/bool8_ref.h>
#	include <vcl/core/simd/bool16_ref.h>
#	include <vcl/core/simd/float4_ref.h>
#	include <vcl/core/simd/float8_ref.h>
#	include <vcl/core/simd/float16_ref.h>
#	include <vcl/core/simd/int4_ref.h>
#	include <vcl/core/simd/int8_ref.h>
#	include <vcl/core/simd/int16_ref.h>
namespace Vcl
{
	//! \addtogroup group_vcl_core_simd
	//! \{

	//! Creating a new scalar vector by choosing elements according to \p mask
	//! \tparam Scalar Scalar values in the vector
	//! \tparam Width Number of entries in the vector
	//! \param mask Boolean vector selecting between \p a and \p b
	//! \param a Values selected when \p mask entries are true
	//! \param b Values selected when \p mask entries are false
	//! \returns The blended vectors \p a and \p b according to \p mask
	template<typename Scalar, int Width>
	VectorScalar<Scalar, Width> select
	(
		const VectorScalar<bool, Width>& mask,
		const VectorScalar<Scalar, Width>& a,
		const VectorScalar<Scalar, Width>& b
	)
	{
		VectorScalar<Scalar, Width> blended;
		for (int i = 0; i < Width; i++)
			blended[i] = mask[i] ? a[i] : b[i];
		
		return blended;
	}

	template<int Width>
	bool any(const VectorScalar<bool, Width>& x)
	{
		bool res = false;
		for (int i = 0; i < Width; i++)
			res = res || x[i];
		
		return res;
	}
	template<int Width>
	bool all(const VectorScalar<bool, Width>& x)
	{
		bool res = true;
		for (int i = 0; i < Width; i++)
			res = res && x[i];

		return res;
	}
	template<int Width>
	bool none(const VectorScalar<bool, Width>& x)
	{
		bool res = false;
		for (int i = 0; i < Width; i++)
			res = res || x[i];

		return !res;
	}
	
	template<typename Scalar, int Width>
	std::ostream& operator<< (std::ostream &s, const VectorScalar<Scalar, Width>& rhs)
	{		
		s << "'" << rhs[0];
		for (int i = 1; i < Width; i++)
			s << ", " << rhs[1];
		s << "'";
		return s;
	}
	//! \}
}
#endif

namespace Vcl
{
	template<typename T>
	struct VectorTypes
	{};

	template<>
	struct VectorTypes<float>
	{
		using float_t = float;
		using int_t = int;
		using bool_t = bool;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 4>>
	{
		using float_t = VectorScalar<float, 4>;
		using int_t   = VectorScalar<int, 4>;
		using bool_t  = VectorScalar<bool, 4>;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 8>>
	{
		using float_t = VectorScalar<float, 8>;
		using int_t = VectorScalar<int, 8>;
		using bool_t = VectorScalar<bool, 8>;
	};

	template<>
	struct VectorTypes<VectorScalar<float, 16>>
	{
		using float_t = VectorScalar<float, 16>;
		using int_t = VectorScalar<int, 16>;
		using bool_t = VectorScalar<bool, 16>;
	};

	template<typename T>
	struct NumericTrait
	{
		using base_t = T;
		using wide_t = T;
	};

	template<>
	struct NumericTrait<VectorScalar<float, 4>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};
	template<>
	struct NumericTrait<VectorScalar<float, 8>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_AVX
		using wide_t = __m256;
#elif defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};
	template<>
	struct NumericTrait<VectorScalar<float, 16>>
	{
		using base_t = float;
#if defined VCL_VECTORIZE_AVX
		using wide_t = __m256;
#elif defined VCL_VECTORIZE_SSE
		using wide_t = __m128;
#elif defined VCL_VECTORIZE_NEON
		using wide_t = float32x4_t;
#endif
	};
	
	//! Component-wise negation
	template<int Width>
	VCL_STRONG_INLINE VectorScalar<float, Width> operator-(const VectorScalar<float, Width>& a)
	{
		return a * VectorScalar<float, Width>(-1);
	}

	//! Component-wise negation
	template<int Width>
	VCL_STRONG_INLINE VectorScalar<int, Width> operator-(const VectorScalar<int, Width>& a)
	{
		return a * VectorScalar<int, Width>(-1);
	}

	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<Scalar, Width> operator +(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) + b;
	}
	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<Scalar, Width> operator -(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) -b;
	}
	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<Scalar, Width> operator *(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) * b;
	}
	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<Scalar, Width> operator /(Scalar a, const VectorScalar<Scalar, Width>& b)
	{
		return VectorScalar<Scalar, Width>(a) / b;
	}

	template<int Width> VCL_STRONG_INLINE Vcl::VectorScalar<bool, Width> isinf(const Vcl::VectorScalar<float, Width>& x) { return x.isinf(); }

	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> abs  (const Vcl::VectorScalar<Scalar, Width>& x) { return x.abs(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> abs2 (const Vcl::VectorScalar<Scalar, Width>& x) { return x*x; }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> sqrt (const Vcl::VectorScalar<Scalar, Width>& x) { return x.sqrt(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> exp  (const Vcl::VectorScalar<Scalar, Width>& x) { return x.exp(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> log  (const Vcl::VectorScalar<Scalar, Width>& x) { return x.log(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> rcp  (const Vcl::VectorScalar<Scalar, Width>& x) { return x.rcp(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> rsqrt(const Vcl::VectorScalar<Scalar, Width>& x) { return x.rsqrt(); }

	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> sin(const Vcl::VectorScalar<Scalar, Width>& x) { return x.sin(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> cos(const Vcl::VectorScalar<Scalar, Width>& x) { return x.cos(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> acos(const Vcl::VectorScalar<Scalar, Width>& x) { return x.acos(); }

	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> pow(const Vcl::VectorScalar<Scalar, Width>& x, const Vcl::VectorScalar<Scalar, Width>& y)  { return exp(log(x) * y); }

	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> min(const Vcl::VectorScalar<Scalar, Width>& x, const Vcl::VectorScalar<Scalar, Width>& y)  { return x.min(y); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> max(const Vcl::VectorScalar<Scalar, Width>& x, const Vcl::VectorScalar<Scalar, Width>& y)  { return x.max(y); }

	template<typename Scalar, int Width> VCL_STRONG_INLINE Scalar min(const Vcl::VectorScalar<Scalar, Width>& x)  { return x.min(); }
	template<typename Scalar, int Width> VCL_STRONG_INLINE Scalar max(const Vcl::VectorScalar<Scalar, Width>& x)  { return x.max(); }

	template<int Width>
	VCL_STRONG_INLINE Vcl::VectorScalar<float, Width> sgn(const Vcl::VectorScalar<float, Width>& x)
	{
		return x.sgn();
	}
	template<int Width>
	VCL_STRONG_INLINE VectorScalar<int, Width> sgn(const VectorScalar<int, Width>& a)
	{
		using int_t = VectorScalar<int, Width>;
		return select(int_t(0) < a, int_t(1), int_t(0)) - select(a < int_t(0), int_t(1), int_t(0));
	}

	VCL_STRONG_INLINE float min(float x)
	{
		return x;
	}

	VCL_STRONG_INLINE float max(float x)
	{
		return x;
	}

	VCL_STRONG_INLINE bool any(bool b)
	{
		return b;
	}

	VCL_STRONG_INLINE bool all(bool b)
	{
		return b;
	}

	VCL_STRONG_INLINE bool none(bool b)
	{
		return !b;
	}

	template<typename SCALAR>
	VCL_STRONG_INLINE SCALAR select(bool mask, const SCALAR& a, const SCALAR& b)
	{
		return mask ? a : b;
	}

	template<typename Scalar, int Width, int Rows, int Cols>
	VCL_STRONG_INLINE Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> select
	(
		const VectorScalar<bool, Width>& mask,
		const Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols>& a,
		const Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols>& b
	)
	{
		Eigen::Matrix<VectorScalar<Scalar, Width>, Rows, Cols> selected;
		for (int c = 0; c < Cols; c++)
		{
			for (int r = 0; r < Rows; r++)
			{
				selected(r, c) = select(mask, a(r, c), b(r, c));
			}
		}

		return selected;
	}
	
	template<typename Scalar, int Width, size_t N>
	VCL_STRONG_INLINE std::array<VectorScalar<Scalar, Width>, N> select
	(
		const VectorScalar<bool, Width>& mask,
		const std::array<VectorScalar<Scalar, Width>, N>& a,
		const std::array<VectorScalar<Scalar, Width>, N>& b
	)
	{
		std::array<VectorScalar<Scalar, Width>, N> selected;

		for (size_t i = 0; i < N; i++)
			selected[i] = select(mask, a[i], b[i]);

		return selected;
	}

	template<typename SCALAR>
	VCL_STRONG_INLINE void cswap(bool mask, SCALAR& a, SCALAR& b)
	{
		SCALAR c = a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}
	template<typename SCALAR>
	VCL_STRONG_INLINE void cnswap(bool mask, SCALAR& a, SCALAR& b)
	{
		SCALAR c = -a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}

	template<typename Scalar, int Width>
	VCL_STRONG_INLINE void cswap(const VectorScalar<bool, Width>& mask, VectorScalar<Scalar, Width>& a, VectorScalar<Scalar, Width>& b)
	{
		VectorScalar<Scalar, Width> c = select(mask, b, a);
		VectorScalar<Scalar, Width> d = select(mask, a, b);
		a = c;
		b = d;
	}
	
	template<typename Scalar, int Width>
	VCL_STRONG_INLINE void cnswap(const VectorScalar<bool, Width>& mask, VectorScalar<Scalar, Width>& a, VectorScalar<Scalar, Width>& b)
	{
		VectorScalar<Scalar, Width> c = -a;
		a = select(mask, b, a);
		b = select(mask, c, b);
	}
	
	template<typename Scalar, int Width>
	VCL_STRONG_INLINE void cswap(const VectorScalar<bool, Width>& mask, Eigen::Matrix<VectorScalar<Scalar, Width>, 3, 1>& a, Eigen::Matrix<VectorScalar<Scalar, Width>, 3, 1>& b)
	{
		cswap(mask, a(0), b(0));
		cswap(mask, a(1), b(1));
		cswap(mask, a(2), b(2));
	}

	template<typename Scalar, int Width>
	VCL_STRONG_INLINE void cnswap(const VectorScalar<bool, Width>& mask, Eigen::Matrix<VectorScalar<Scalar, Width>, 3, 1>& a, Eigen::Matrix<VectorScalar<Scalar, Width>, 3, 1>& b)
	{
		cnswap(mask, a(0), b(0));
		cnswap(mask, a(1), b(1));
		cnswap(mask, a(2), b(2));
	}

	template<typename Scalar, int Width>
	VCL_STRONG_INLINE VectorScalar<bool, Width> equal
	(
		const VectorScalar<Scalar, Width>& x,
		const VectorScalar<Scalar, Width>& y,
		const VectorScalar<Scalar, Width>& tol = 0
	)
	{
		return abs(x - y) <= tol * max(VectorScalar<Scalar, Width>(1), max(abs(x), abs(y)));
	}

	typedef VectorScalar<float,  4> float4;
	typedef VectorScalar<float,  8> float8;
	typedef VectorScalar<float, 16> float16;
	typedef VectorScalar<float, 32> float32;

	typedef VectorScalar<int,  4> int4;
	typedef VectorScalar<int,  8> int8;
	typedef VectorScalar<int, 16> int16;
	typedef VectorScalar<int, 32> int32;
	
	typedef VectorScalar<bool,  4> bool4;
	typedef VectorScalar<bool,  8> bool8;
	typedef VectorScalar<bool, 16> bool16;
	typedef VectorScalar<bool, 32> bool32;
}

namespace Eigen
{
	template<> struct NumTraits<Vcl::float4> : GenericNumTraits<Vcl::float4>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static float dummy_precision() { return 1e-5f; }
	};
	template<> struct NumTraits<Vcl::float8> : GenericNumTraits<Vcl::float8>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static float dummy_precision() { return 1e-5f; }
	};
	template<> struct NumTraits<Vcl::float16> : GenericNumTraits<Vcl::float16>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static float dummy_precision() { return 1e-5f; }
	};
	template<> struct NumTraits<Vcl::float32> : GenericNumTraits<Vcl::float32>
	{
		enum
		{
			IsInteger = std::numeric_limits<float>::is_integer,
			IsSigned = std::numeric_limits<float>::is_signed,
			IsComplex = 0,
			RequireInitialization = internal::is_arithmetic<float>::value ? 0 : 1,
			ReadCost = 1,
			AddCost = 1,
			MulCost = 1
		};

		EIGEN_STRONG_INLINE static float dummy_precision() { return 1e-5f; }
	};
}

namespace Eigen { namespace numext
{
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE const Vcl::VectorScalar<Scalar, Width>& conj(const Vcl::VectorScalar<Scalar, Width>& x)  { return x; }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE const Vcl::VectorScalar<Scalar, Width>& real(const Vcl::VectorScalar<Scalar, Width>& x)  { return x; }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> imag(const Vcl::VectorScalar<Scalar, Width>&)   { return Vcl::VectorScalar<Scalar, Width>(0); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> isinf (const Vcl::VectorScalar<Scalar, Width>& x) { return x.isinf(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> abs2(const Vcl::VectorScalar<Scalar, Width>& x) { return x*x; }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> sqrt(const Vcl::VectorScalar<Scalar, Width>& x) { return x.sqrt(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> exp (const Vcl::VectorScalar<Scalar, Width>& x) { return x.exp(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> log (const Vcl::VectorScalar<Scalar, Width>& x) { return x.log(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> sin (const Vcl::VectorScalar<Scalar, Width>& x) { return x.sin(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> cos (const Vcl::VectorScalar<Scalar, Width>& x) { return x.cos(); }
	template<typename Scalar, int Width> EIGEN_STRONG_INLINE Vcl::VectorScalar<Scalar, Width> pow (const Vcl::VectorScalar<Scalar, Width>& x, const Vcl::VectorScalar<Scalar, Width>& y)  { return x.pow(y); }
}}
