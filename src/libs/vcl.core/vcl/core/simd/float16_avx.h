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
#include <vcl/core/simd/bool16_avx.h>
#include <vcl/core/simd/intrinsics_avx.h>
#include <vcl/core/simd/vectorscalar.h>

namespace Vcl
{
	template<>
	class alignas(32) VectorScalar<float, 16> : protected Core::Simd::VectorScalarBase<float, 16, Core::Simd::SimdExt::AVX>
	{
	public:
		VCL_SIMD_VECTORSCALAR_SETUP(AVX)

	public:
		VCL_SIMD_BINARY_OP(operator+, _mm256_add_ps, 2)
		VCL_SIMD_BINARY_OP(operator-, _mm256_sub_ps, 2)
		VCL_SIMD_BINARY_OP(operator*, _mm256_mul_ps, 2)
		VCL_SIMD_BINARY_OP(operator/, _mm256_div_ps, 2)
		
	public:
		VCL_SIMD_ASSIGN_OP(operator+=, _mm256_add_ps, 2)
		VCL_SIMD_ASSIGN_OP(operator-=, _mm256_sub_ps, 2)
		VCL_SIMD_ASSIGN_OP(operator*=, _mm256_mul_ps, 2)
		VCL_SIMD_ASSIGN_OP(operator/=, _mm256_div_ps, 2)
		
	public:
		VCL_SIMD_COMP_OP(operator==, _mm256_cmpeq_ps,  2)
		VCL_SIMD_COMP_OP(operator!=, _mm256_cmpneq_ps, 2)
		VCL_SIMD_COMP_OP(operator<,  _mm256_cmplt_ps,  2)
		VCL_SIMD_COMP_OP(operator<=, _mm256_cmple_ps,  2)
		VCL_SIMD_COMP_OP(operator>,  _mm256_cmpgt_ps,  2)
		VCL_SIMD_COMP_OP(operator>=, _mm256_cmpge_ps,  2)

	public:
		VCL_SIMD_UNARY_OP(abs, _mm256_abs_ps, 2)
		VCL_SIMD_UNARY_OP(sgn, _mm256_sgn_ps, 2)

		VCL_SIMD_UNARY_OP(sin, _mm256_sin_ps, 2)
		VCL_SIMD_UNARY_OP(cos, _mm256_cos_ps, 2)
		VCL_SIMD_UNARY_OP(acos, _mm256_acos_ps, 2)

		VCL_SIMD_UNARY_OP(exp, _mm256_exp_ps, 2)
		VCL_SIMD_UNARY_OP(log, _mm256_log_ps, 2)
		VCL_SIMD_UNARY_OP(sqrt, _mm256_sqrt_ps, 2)
		VCL_SIMD_UNARY_OP(rcp, _mmVCL_rcp_ps, 2)
		VCL_SIMD_UNARY_OP(rsqrt, _mmVCL_rsqrt_ps, 2)
		
		VCL_SIMD_QUERY_OP(isinf, _mm256_isinf_ps, 2)

	public:
		VCL_SIMD_BINARY_OP(min, _mm256_min_ps, 2)
		VCL_SIMD_BINARY_OP(max, _mm256_max_ps, 2)

		VCL_SIMD_BINARY_REDUCTION_OP(dot, _mmVCL_dp_ps, Core::Simd::Details::add, 2)

		VCL_SIMD_UNARY_REDUCTION_OP(min, _mmVCL_hmin_ps, Mathematics::min, 2)
		VCL_SIMD_UNARY_REDUCTION_OP(max, _mmVCL_hmax_ps, Mathematics::max, 2)
	};

	VCL_STRONG_INLINE VectorScalar<float, 16> select(const VectorScalar<bool, 16>& mask, const VectorScalar<float, 16>& a, const VectorScalar<float, 16>& b) noexcept
	{
		// (((b ^ a) & mask)^b)
		return VectorScalar<float, 16>
		(
			_mm256_blendv_ps(b.get(0), a.get(0), mask.get(0)),
			_mm256_blendv_ps(b.get(1), a.get(1), mask.get(1))
		);
	}

	VCL_STRONG_INLINE std::ostream& operator<< (std::ostream &s, const VectorScalar<float, 16>& rhs)
	{
		alignas(32) float vars[16];
		_mm256_store_ps(vars + 0, rhs.get(0));
		_mm256_store_ps(vars + 8, rhs.get(1));
		
		s << "'" << vars[0] << ", " << vars[1] << ", " << vars[2] << ", " << vars[3]
				 << vars[4] << ", " << vars[5] << ", " << vars[6] << ", " << vars[7] << "'";

		return s;
	}
}
