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

// VCL
#include <vcl/core/simd/bool16_ref.h>
#include <vcl/core/simd/common.h>
#include <vcl/core/simd/vectorscalar.h>
#include <vcl/math/math.h>

namespace Vcl {
	template<>
	class VectorScalar<float, 16> : protected Core::Simd::VectorScalarBase<float, 16, Core::Simd::SimdExt::None>
	{
	public:
		VCL_SIMD_VECTORSCALAR_SETUP(None)
		explicit VectorScalar(const Scalar* scalars, size_t stride)
		{
			for (int i = 0; i < NrValues; i++)
				_data[i] = scalars[i * stride];
		}
		VCL_STRONG_INLINE Scalar& operator[](int idx) { return _data[idx]; }

	public:
		VCL_SIMD_BINARY_OP(operator+, Core::Simd::Details::add, 16)
		VCL_SIMD_BINARY_OP(operator-, Core::Simd::Details::sub, 16)
		VCL_SIMD_BINARY_OP(operator*, Core::Simd::Details::mul, 16)
		VCL_SIMD_BINARY_OP(operator/, Core::Simd::Details::div, 16)

	public:
		VCL_SIMD_ASSIGN_OP(operator+=, Core::Simd::Details::add, 16)
		VCL_SIMD_ASSIGN_OP(operator-=, Core::Simd::Details::sub, 16)
		VCL_SIMD_ASSIGN_OP(operator*=, Core::Simd::Details::mul, 16)
		VCL_SIMD_ASSIGN_OP(operator/=, Core::Simd::Details::div, 16)

	public:
		VCL_SIMD_COMP_OP(operator==, Core::Simd::Details::cmpeq, 16)
		VCL_SIMD_COMP_OP(operator!=, Core::Simd::Details::cmpne, 16)
		VCL_SIMD_COMP_OP(operator<, Core::Simd::Details::cmplt, 16)
		VCL_SIMD_COMP_OP(operator<=, Core::Simd::Details::cmple, 16)
		VCL_SIMD_COMP_OP(operator>, Core::Simd::Details::cmpgt, 16)
		VCL_SIMD_COMP_OP(operator>=, Core::Simd::Details::cmpge, 16)

	public:
		VCL_SIMD_UNARY_OP(abs, std::abs, 16)
		VCL_SIMD_UNARY_OP(sgn, Vcl::Mathematics::sgn, 16)

		VCL_SIMD_UNARY_OP(sin, std::sin, 16)
		VCL_SIMD_UNARY_OP(cos, std::cos, 16)
		VCL_SIMD_UNARY_OP(acos, std::acos, 16)

		VCL_SIMD_UNARY_OP(exp, std::exp, 16)
		VCL_SIMD_UNARY_OP(log, std::log, 16)
		VCL_SIMD_UNARY_OP(sqrt, std::sqrt, 16)
		VCL_SIMD_UNARY_OP(rcp, Vcl::Mathematics::rcp, 16)
		VCL_SIMD_UNARY_OP(rsqrt, Vcl::Mathematics::rsqrt, 16)

		VCL_SIMD_QUERY_OP(isinf, std::isinf, 16)

	public:
		VCL_SIMD_BINARY_OP(min, std::min, 16)
		VCL_SIMD_BINARY_OP(max, std::max, 16)

		VCL_SIMD_BINARY_REDUCTION_OP(dot, Core::Simd::Details::mul, Core::Simd::Details::add, 16)

		VCL_SIMD_UNARY_REDUCTION_OP(min, Core::Simd::Details::nop, std::min, 16)
		VCL_SIMD_UNARY_REDUCTION_OP(max, Core::Simd::Details::nop, std::max, 16)
	};
}
